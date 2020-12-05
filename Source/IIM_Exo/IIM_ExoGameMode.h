// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IIM_ExoGameMode.generated.h"

UCLASS(minimalapi)
class AIIM_ExoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIIM_ExoGameMode();

	UFUNCTION()
	void Respawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Test();

};



