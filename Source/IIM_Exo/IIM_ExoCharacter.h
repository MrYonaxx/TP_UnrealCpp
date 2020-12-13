// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Delegates/Delegate.h" 
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickupComponent.h"
#include "Components/TimelineComponent.h"
#include "IIM_ExoCharacter.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShootDelegate, int, Ammo);

UENUM(BlueprintType)
enum class CharacterState : uint8 {
	IDLE = 0	UMETA(DisplayName = "IDLE"),
	CROUCH = 1  UMETA(DisplayName = "CROUCH"),
	JUMP = 2    UMETA(DisplayName = "JUMP"),
	HIT = 3		UMETA(DisplayName = "HIT"),
};



UCLASS(config=Game)
class AIIM_ExoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UPickupComponent* PickupComponent;

	// Note : component ou system faut se décider on s'y perd
	// Putain de unreal de ta mère à pas bugguer sur les component inherited, 
	// Moi du futur, faut recompiler unreal en virant le UProperty puis le remettre
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UInventorySystem* Inventory;

	FVector MoveDirection;

	class UUserWidget* CurrentWidget;

	class AItemWeapon* weaponEquiped;

	bool aimOn = false;
	bool reloading = false;
	bool isHit = false;
	FTimeline TimelineAimZoom;
	FTimeline TimelineAimDezoom;

public:

	AIIM_ExoCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseCameraLength;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float ZoomCameraLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float health;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FName PlayerWeaponSocketName = TEXT("RightHandSocket");

	UPROPERTY(BlueprintAssignable, Category = "Test")
	FTestDelegate OnDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UUserWidget> pauseHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* curveFloat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* curveFloatReverse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	UAnimMontage* ReloadMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	UAnimMontage* HitMontage;

	UPROPERTY(BlueprintAssignable, Category = "Test")
	FShootDelegate OnShoot;


protected:

	/** Resets HMD orientation in VR. */
	//void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void OnFire();

	void Aim();
	void StopAim();

	void PickObject();

	void Reload();
	void EndReload(UAnimMontage* Montage, bool bInterrupted);

	// à bouger ?
	void PauseGame();
	UFUNCTION(BlueprintCallable)
	void ResumeGame();




protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void Tick(float deltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UPickupComponent* GetPickupComponent() const { return PickupComponent; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UInventorySystem* GetInventory() const { return Inventory; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AItemWeapon* GetWeapon() const { return weaponEquiped; }

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float damage);
	void EndTakeDamage(UAnimMontage* Montage, bool bInterrupted);
	void CharaCrouch();

	UFUNCTION(BlueprintCallable)
	FVector GetMoveDirection();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<class AItemWeapon> weapon);

	UFUNCTION()
	void TimelineZoomProgress(float val);

};

