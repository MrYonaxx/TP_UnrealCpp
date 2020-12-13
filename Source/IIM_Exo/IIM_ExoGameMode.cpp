// Copyright Epic Games, Inc. All Rights Reserved.

#include "IIM_ExoGameMode.h"
#include "IIM_ExoCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"

AIIM_ExoGameMode::AIIM_ExoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;

	}
	respawnLocation = FVector(-920.0f, 8.0f, 120.0f);

}

void AIIM_ExoGameMode::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,TEXT("Allo"));

	// ça crash ça de temps en temps, ptet le pawn est pas initialisé encore et faut attendre 1 frame


	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AIIM_ExoGameMode::Test, 2, false);
}

void AIIM_ExoGameMode::Test() 
{
	AIIM_ExoCharacter* tmp = Cast<AIIM_ExoCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	tmp->OnDeath.AddDynamic(this, &AIIM_ExoGameMode::Respawn);

	/*AIIM_ExoCharacter* tmp = Cast<AIIM_ExoCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	tmp->OnDeath.AddDynamic(this, &AIIM_ExoGameMode::Respawn);
	tmp->Destroy();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("A"));*/
}

void AIIM_ExoGameMode::Respawn() 
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Ded1"));

	//FVector Location(-770.0f, 370.0f, 240.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	AIIM_ExoCharacter* tmp = GetWorld()->SpawnActor<AIIM_ExoCharacter>(DefaultPawnClass, respawnLocation, Rotation, SpawnInfo);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(tmp);
	tmp->OnDeath.AddDynamic(this, &AIIM_ExoGameMode::Respawn);

}

void AIIM_ExoGameMode::SetRespawnLocation(FVector newPosition)
{
	respawnLocation = newPosition;
}