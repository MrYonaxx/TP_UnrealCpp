// Fill out your copyright notice in the Description page of Project Settings.

#include "IIM_ExoCharacter.h"
#include "Pickup_Object.h"
#include "InventorySystem.h"

// Sets default values
APickup_Object::APickup_Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickup_Object::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup_Object::Interact(AIIM_ExoCharacter* user)
{
	/*for (size_t i = objectNumber; i > 0; i--)
	{
		if (user->GetInventory()->AddItem(itemData) == true) 
		{
			objectNumber -= 1;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Allo"));
	if(objectNumber == 0)
		Destroy();*/
}


void APickup_Object::InteractProxy(AIIM_ExoCharacter* user)
{
	for (size_t i = objectNumber; i > 0; i--)
	{
		if (user->GetInventory()->AddItem(itemData) == true)
		{
			objectNumber -= 1;
		}
	}
	if (objectNumber == 0)
		Destroy();
}

void APickup_Object::SetPhysics(bool b)
{
	mesh->SetSimulatePhysics(b);
}
