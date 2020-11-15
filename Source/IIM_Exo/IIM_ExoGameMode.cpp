// Copyright Epic Games, Inc. All Rights Reserved.

#include "IIM_ExoGameMode.h"
#include "IIM_ExoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AIIM_ExoGameMode::AIIM_ExoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
