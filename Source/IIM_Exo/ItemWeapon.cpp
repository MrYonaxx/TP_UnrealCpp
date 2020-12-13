// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "ItemWeapon.h"



AItemWeapon::AItemWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	Mesh->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(Mesh);

}

void AItemWeapon::Shoot(FRotator SpawnRotation)
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			//const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = MuzzleLocation->GetComponentLocation();//((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			World->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			ammoLoaded -= 1;
		}
	}
}

bool AItemWeapon::CanShoot()
{
	return (ammoLoaded != 0);
	//return true;
}

void AItemWeapon::SetPhysics(bool b)
{
	Mesh->SetSimulatePhysics(b);
}

int AItemWeapon::GetAmmoLoaded()
{
	return ammoLoaded;
}

bool AItemWeapon::IsLoaded()
{
	return (ammoLoaded != 0);
}

void AItemWeapon::Reload(int ammo)
{
	ammoLoaded += ammo;
}
