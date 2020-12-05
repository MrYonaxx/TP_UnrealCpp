// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Delegates/Delegate.h" //à inclure pour que ça compile parce que c'est comme ça

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IIM_ExoEnemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate2);

UCLASS()
class IIM_EXO_API AIIM_ExoEnemy : public ACharacter
{
	GENERATED_BODY()
	int indexWaypoint;

public:
	// Sets default values for this character's properties
	AIIM_ExoEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float health;

	UPROPERTY(EditAnywhere, Category = Patrol)
	TArray<AActor*> waypoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Patrol)
	bool randomPoint;

	UPROPERTY(BlueprintAssignable)
	FTestDelegate2 OnDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FVector GetWaypoint();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float damage);
};
