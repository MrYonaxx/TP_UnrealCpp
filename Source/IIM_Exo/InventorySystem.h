// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Delegates/Delegate.h" 
#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventorySystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryUpdated, UItemDataAsset*, Item, int, Quantity);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IIM_EXO_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

	// fallait faire une struct mais flemme
	TArray<int> itemQuantity;
	TArray<UItemDataAsset*> inventory;

public:	
	// Sets default values for this component's properties
	UInventorySystem();





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int inventoryCapacity;

	UPROPERTY(BlueprintAssignable, Category = "Test")
	FInventoryUpdated OnInventoryUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemDataAsset* item, bool broadcast = true);
	UFUNCTION(BlueprintCallable)
	int AddItemMultiple(UItemDataAsset* item, int number);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UItemDataAsset* item, bool broadcast = true);

	UFUNCTION(BlueprintCallable)
	int GetTotalItemQuantity(UItemDataAsset* item);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<UItemDataAsset*> GetItems() const { return inventory; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<int> GetItemsQuantity() const { return itemQuantity; }

	UFUNCTION(BlueprintCallable)
	void SetItems(TArray<UItemDataAsset*> newInventory) { inventory = newInventory; }

	UFUNCTION(BlueprintCallable)
	void SetItemsQuantity(TArray<int> newInventoryQuantity) { itemQuantity = newInventoryQuantity; }


	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetItemQuantity(int index) const { return itemQuantity[index]; }
		
};
