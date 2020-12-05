// Fill out your copyright notice in the Description page of Project Settings.

#include "IIM_ExoCharacter.h"
#include "DamageZone.h"
#include "Components/BoxComponent.h"

// Sets default values
ADamageZone::ADamageZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	boxCollision->InitBoxExtent(FVector(5, 5, 5));
}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
	Super::BeginPlay();
	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnActorBeginOverlap);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnActorEndOverlap);
}

// Called every frame
void ADamageZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (characterDamaging != nullptr) 
	{
		t -= DeltaTime;
		if (t <= 0) 
		{
			characterDamaging->TakeDamage(damage);
			t = tickDelay;
		}
	}
}


void ADamageZone::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AIIM_ExoCharacter* player = Cast<AIIM_ExoCharacter>(OtherActor);
	if (player != nullptr)
	{
		characterDamaging = player;
		t = tickDelay;
	}
}

void ADamageZone::OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AIIM_ExoCharacter* player = Cast<AIIM_ExoCharacter>(OtherActor);
	if (player != nullptr)
	{
		characterDamaging = nullptr;
		//t = tickDelay;
	}
}

