// Fill out your copyright notice in the Description page of Project Settings.


#include "IIM_ExoEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AIIM_ExoEnemy::AIIM_ExoEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 90.0f, 0.0f); // ...at this rotation rate

	health = 6;
	randomPoint = false;
	indexWaypoint = 0;
}

// Called when the game starts or when spawned
void AIIM_ExoEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIIM_ExoEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIIM_ExoEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector AIIM_ExoEnemy::GetWaypoint()
{
	/*if (waypoints == nullptr)
		return GetActorLocation();*/
	if (waypoints.Num() == 0)
		return GetActorLocation();
	indexWaypoint += 1;
	if (indexWaypoint >= waypoints.Num())
		indexWaypoint = 0;
	return waypoints[indexWaypoint]->GetActorLocation();
}

void AIIM_ExoEnemy::TakeDamage(float damage)
{
	health -= damage;
	if (health <= 0)
	{
		OnDeath.Broadcast();
		//GetCapsuleComponent()->
	}
}