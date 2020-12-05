// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

UCLASS()
class IIM_EXO_API ADamageZone : public AActor
{
	GENERATED_BODY()

	float t;
	
public:	
	// Sets default values for this actor's properties
	ADamageZone();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter)
	float damage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter)
	float tickDelay = 5;

	//class UArray<>
	class AIIM_ExoCharacter* characterDamaging = nullptr;

	//UPROPERTY(VisibleDefaultsOnly, Category = Config)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* boxCollision = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
