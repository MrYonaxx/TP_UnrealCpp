// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class IIM_EXO_API AItemWeapon : public AItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;


	int ammoLoaded = 0;

public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemDataAsset* AmmoData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int magazineSize = 8;


public:

	AItemWeapon();
	void Shoot(FRotator SpawnRotation);
	bool CanShoot();
	void SetPhysics(bool b);

	int GetAmmoLoaded();
	bool IsLoaded();
	void Reload(int ammo);

	
};
