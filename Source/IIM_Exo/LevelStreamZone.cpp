// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "LevelStreamZone.h"

#include "IIM_ExoCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelStreamZone::ALevelStreamZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Streaming Trigger"));
	RootComponent = trigger;



}

// Called when the game starts or when spawned
void ALevelStreamZone::BeginPlay()
{
	Super::BeginPlay();
	trigger->OnComponentBeginOverlap.AddDynamic(this, &ALevelStreamZone::BeginOverlap);
	trigger->OnComponentEndOverlap.AddDynamic(this, &ALevelStreamZone::EndOverlap);
	
}

// Called every frame
void ALevelStreamZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelStreamZone::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AIIM_ExoCharacter* player = Cast<AIIM_ExoCharacter>(OtherActor);
	if (player != nullptr && levelName != "")
	{
		FLatentActionInfo info;
		UGameplayStatics::LoadStreamLevel(this, levelName, true, false, info);
	}
}

void ALevelStreamZone::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AIIM_ExoCharacter* player = Cast<AIIM_ExoCharacter>(OtherActor);
	if (player != nullptr)
	{
		FLatentActionInfo info;
		UGameplayStatics::UnloadStreamLevel(this, levelName, info, false);
	}
}