// Copyright Epic Games, Inc. All Rights Reserved.

#include "IIM_ExoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "BaseProjectile.h"
#include "ItemWeapon.h"
#include "InventorySystem.h"

#include "DrawDebugHelpers.h"


#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"

#include "Components/TimelineComponent.h"

//////////////////////////////////////////////////////////////////////////
// AIIM_ExoCharacter

AIIM_ExoCharacter::AIIM_ExoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/*MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));*/

	PickupComponent = CreateDefaultSubobject<UPickupComponent>(TEXT("PickupComponent2"));
	PickupComponent->SetupAttachment(CameraBoom);

	Inventory = CreateDefaultSubobject<UInventorySystem>(TEXT("InventoryComponent"));

	MoveDirection = FVector(0.0f, 0.0f, 0.0f);

	BaseCameraLength = 150.0f;
	ZoomCameraLength = 60.0f;
	aimOn = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AIIM_ExoCharacter::Tick(float deltaTime)
{

	ACharacter::Tick(deltaTime);

	// C'est un peu compliqué pour rien, ptet faire un lerp à la main
	TimelineAimZoom.TickTimeline(deltaTime);
	TimelineAimDezoom.TickTimeline(deltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AIIM_ExoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AIIM_ExoCharacter::PickObject);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AIIM_ExoCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AIIM_ExoCharacter::StopAim);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AIIM_ExoCharacter::OnFire);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AIIM_ExoCharacter::CharaCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACharacter::UnCrouch);

	PlayerInputComponent->BindAxis("MoveForward", this, &AIIM_ExoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AIIM_ExoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AIIM_ExoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AIIM_ExoCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AIIM_ExoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AIIM_ExoCharacter::TouchStopped);


	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AIIM_ExoCharacter::PauseGame);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AIIM_ExoCharacter::OnResetVR);
}


/*void AIIM_ExoCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}*/

void AIIM_ExoCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AIIM_ExoCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AIIM_ExoCharacter::TurnAtRate(float Rate)
{

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	const FRotator Rotation2 = GetControlRotation();
	const FRotator YawRotation2(0, Rotation2.Yaw, 0);
	SetActorRotation(YawRotation2);
}

void AIIM_ExoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

	const FRotator Rotation2 = GetControlRotation();
	const FRotator YawRotation2(0, Rotation2.Yaw, 0);
	SetActorRotation(YawRotation2);
}

void AIIM_ExoCharacter::MoveForward(float Value)
{
	if (bIsCrouched == true)
		return;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (aimOn == true) 
		{
			Value *= 0.25f;
		}
		else if (Value <= -0.5f)
			Value = -0.5f;
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		const FRotator Rotation2 = GetControlRotation();
		const FRotator YawRotation2(0, Rotation2.Yaw, 0);
		SetActorRotation(YawRotation2);
		//MoveDirection.Y = Value;
	}
	MoveDirection.Y = Value;
}

void AIIM_ExoCharacter::MoveRight(float Value)
{
	if (bIsCrouched == true)
		return;
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		Value = FMath::Clamp(Value, -0.5f, 0.5f);
		if (aimOn == true)
		{
			Value *= 0.25f;
		}
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		const FRotator Rotation2 = GetControlRotation();
		const FRotator YawRotation2(0, Rotation2.Yaw, 0);
		SetActorRotation(YawRotation2);

		//MoveDirection.X = Value;
	}
	MoveDirection.X = Value;
}


FVector AIIM_ExoCharacter::GetMoveDirection() 
{
	return MoveDirection;
}






// ============================================================
void AIIM_ExoCharacter::EquipWeapon(TSubclassOf<class AItemWeapon> weapon)
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	weaponEquiped = GetWorld()->SpawnActor<AItemWeapon>(weapon, FVector(0.0f), FRotator(0.0f), ActorSpawnParams);

	weaponEquiped->SetPhysics(false);
	weaponEquiped->SetActorEnableCollision(false);
	FAttachmentTransformRules rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	rule.bWeldSimulatedBodies = false;
	rule.LocationRule = EAttachmentRule::SnapToTarget;
	rule.RotationRule = EAttachmentRule::SnapToTarget;
	rule.ScaleRule = EAttachmentRule::KeepWorld;
	weaponEquiped->AttachToComponent(GetMesh(), rule, PlayerWeaponSocketName);
}

void AIIM_ExoCharacter::OnFire()
{
	if (weaponEquiped != nullptr) 
	{
		if (weaponEquiped->CanShoot()) 
		{
			weaponEquiped->Shoot(GetControlRotation());
		}
		else 
		{
			int ammoToAdd = 0;
			for (size_t i = 0; i < weaponEquiped->magazineSize; i++)
			{
				if (Inventory->RemoveItem(weaponEquiped->AmmoData) == false)
					break;
				ammoToAdd += 1;
			}
			if (ammoToAdd != 0)
			{
				Reload();
				weaponEquiped->Reload(ammoToAdd);
			}
		}
	}
}

void AIIM_ExoCharacter::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Reload"));
}




void AIIM_ExoCharacter::Aim()
{
	aimOn = true;

	TimelineAimDezoom.Stop();

	FOnTimelineFloat timelineProgress;
	timelineProgress.BindUFunction(this, FName("TimelineZoomProgress"));

	TimelineAimZoom.AddInterpFloat(curveFloat, timelineProgress);
	TimelineAimZoom.PlayFromStart();
}


void AIIM_ExoCharacter::StopAim()
{
	aimOn = false;

	TimelineAimZoom.Stop();

	FOnTimelineFloat timelineProgress;
	timelineProgress.BindUFunction(this, FName("TimelineZoomProgress"));

	TimelineAimDezoom.AddInterpFloat(curveFloatReverse, timelineProgress);
	TimelineAimDezoom.PlayFromStart();
}

void AIIM_ExoCharacter::TimelineZoomProgress(float val)
{
	CameraBoom->TargetArmLength = BaseCameraLength - ((BaseCameraLength - ZoomCameraLength) * val);
}


void AIIM_ExoCharacter::PickObject() 
{
	IIInteractable* s = PickupComponent->InteractWithFirstObject();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("J'interagis"));
	if (s)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("J'interagis bien"));
		s->Execute_Interact(Cast<UObject>(s), this);
		//PickupComponent->RemoveObjectFromList();
	}
	//PickupComponent->InteractWithFirstObject()->Execute_Interact(this);
	/*if (objectPicked == nullptr)
	{
		FVector Start = FollowCamera->GetComponentLocation();
		FVector ForwardVector = FollowCamera->GetForwardVector();
		FVector End = (Start + (ForwardVector * 300.f));

		FHitResult Hit;
		FComponentQueryParams DefaultComponentQueryParams;
		FCollisionResponseParams DefaultResponseParam;

		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

		//GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat(), ECC_Visibility, FCollisionShape::MakeSphere(10), DefaultComponentQueryParams, DefaultResponseParam)

			//if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DefaultComponentQueryParams, DefaultResponseParam))
		if(GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat(), ECC_Visibility, FCollisionShape::MakeSphere(20), DefaultComponentQueryParams, DefaultResponseParam))
		{
			APickup_Object* tmp = Cast<APickup_Object>(Hit.GetActor());
			if (tmp != nullptr)
			{
				objectPicked = tmp;
				objectPicked->SetPhysics(false);
				objectPicked->SetActorEnableCollision(false);
				FAttachmentTransformRules rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
				rule.bWeldSimulatedBodies = false;
				rule.LocationRule = EAttachmentRule::SnapToTarget;
				rule.RotationRule = EAttachmentRule::SnapToTarget;
				rule.ScaleRule = EAttachmentRule::KeepWorld;
				objectPicked->AttachToComponent(GetMesh(), rule, PlayerWeaponSocketName);

			}
		}
	}
	else 
	{
		objectPicked->SetPhysics(true);
		objectPicked->SetActorEnableCollision(true);
		FDetachmentTransformRules rule(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
		objectPicked->DetachAllSceneComponents(GetMesh(), rule);
		objectPicked = nullptr;
	}*/


}





// ============================================================
void AIIM_ExoCharacter::TakeDamage(float damage) 
{
	health -= damage;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%f"), health));
	if (health <= 0) 
	{
		// Death
		OnDeath.Broadcast();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Ded2"));

		// Unpick function
		/*if (objectPicked != nullptr)
		{
			objectPicked->SetPhysics(true);
			objectPicked->SetActorEnableCollision(true);
			FDetachmentTransformRules rule(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
			objectPicked->DetachAllSceneComponents(GetMesh(), rule);
			objectPicked = nullptr;
		}*/
		Destroy();
		
	}
}

// Le nom c'est pour pas masqué la fonction parent
void AIIM_ExoCharacter::CharaCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
}


















void AIIM_ExoCharacter::PauseGame()
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), pauseHUD);
	CurrentWidget->AddToViewport();

	FInputModeUIOnly mode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(mode);
	//Ptit mémo : Ok donc UgameplayStatic c'est optionnel en fait
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void AIIM_ExoCharacter::ResumeGame()
{
	CurrentWidget->RemoveFromViewport();
	FInputModeGameOnly mode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(mode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}
