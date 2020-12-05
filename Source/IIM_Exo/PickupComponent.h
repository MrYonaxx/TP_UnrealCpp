// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "IIM_ExoCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "IInteractable.h"
#include "PickupComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), hideCategories = (Mobility))
class IIM_EXO_API UPickupComponent : public USceneComponent
{
	GENERATED_BODY()

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* boxComponent;*/

public:	
	// Sets default values for this component's properties
	UPickupComponent();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Liste d'interface ça ne marche pas apparemment, nouvelle règle
//	TScriptInterface<IIInteractable>
	TArray<AActor*> listInteractables;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// C'est cassé, je comprend rien, ça invente des règles

	UFUNCTION(BlueprintCallable)
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	class IIInteractable* InteractWithFirstObject();// AIIM_ExoCharacter* user);
	void RemoveObjectFromList();
		
};
