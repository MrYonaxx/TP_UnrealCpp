// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"

// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
	inventoryCapacity = 9;
}

bool UInventorySystem::AddItem(UItemDataAsset* item)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hmm"));
	if (item == nullptr)
		return false;
	int position = -1;

	for (size_t i = inventoryCapacity-1; i > 0; i--)
	{
		if (inventory[i] == nullptr) // L'emplacement est vide donc on peut éventuellment placer l'objet ici
		{
			position = i;
		}
		else if (inventory[i] == item)
		{
			if (itemQuantity[i] < inventory[i]->maxStack) // On peut stacker l'objet
			{
				position = i;
				break;
			}
		}
	}
	if (position == -1) // Inventaire plein
		return false;
	inventory[position] = item;
	itemQuantity[position] += 1;
	return true;
}

bool UInventorySystem::RemoveItem(UItemDataAsset* item)
{
	if (item == nullptr)
		return false;

	for (size_t i = 0; i < inventoryCapacity; i++)
	{
		if (inventory[i] == item)
		{
			itemQuantity[i] -= 1;
			if (itemQuantity[i] == 0)
				inventory[i] = nullptr;
			return true;
		}
	}
	return false; // L'objet n'est pas dans l'inventaire
}


// Called when the game starts
void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for (size_t i = 0; i < inventoryCapacity; i++)
	{
		itemQuantity.Add(0);
		inventory.Add(nullptr);
	}
	
}


// Called every frame
/*void UInventorySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

