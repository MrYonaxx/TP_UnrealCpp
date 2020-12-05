// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "Pickup_Object.generated.h"

UCLASS()
class IIM_EXO_API APickup_Object : public AActor, public IIInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* mesh = nullptr;

public:	
	// Sets default values for this actor's properties
	APickup_Object();
	void SetPhysics(bool b);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemDataAsset* itemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int objectNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InteractOverlap();

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteract")
	void InteractEnd();

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteract")
	void Interact(AIIM_ExoCharacter* user);

	UFUNCTION(BlueprintCallable)
	void InteractProxy(AIIM_ExoCharacter* user);

};
