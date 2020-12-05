// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IIM_ExoCharacter.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIM_EXO_API IIInteractable // oups le nom
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteract")
	void InteractOverlap();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteract")
	void InteractEnd();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteract")
	void Interact(AIIM_ExoCharacter* user);
};
