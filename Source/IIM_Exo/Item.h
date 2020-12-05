// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "Item.generated.h"


// Note :
// Data asset = SO de Unity, on peut chercher des assets etc...
// Data Table = Experience curve, stat 



USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName itemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* itemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int maxStack;

};


UCLASS(BlueprintType)
class IIM_EXO_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName itemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* itemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int maxStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AItem> itemBehaviour;
};



UCLASS()
class IIM_EXO_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* itemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName itemID;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
