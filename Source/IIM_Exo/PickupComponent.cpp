// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup_Object.h"
#include "IIM_ExoCharacter.h"
#include "PickupComponent.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBox"));
	//boxComponent->SetupAttachment(this);

	//boxComponent->OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::BeginOverlap);        // set up a notification for when this component overlaps something
	//boxComponent->OnComponentEndOverlap.AddDynamic(this, &UPickupComponent::EndOverlap);

	/*boxComponent->SetRelativeLocation(FVector(250.0f, 0.0f, 0.0f));
	boxComponent->SetRelativeScale3D(FVector(5.0f, 1.0f, 1.0f));*/
	// ...
}


// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//boxComponent->OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::BeginOverlap);
	//boxComponent->OnComponentEndOverlap.AddDynamic(this, &UPickupComponent::EndOverlap);
}


// Called every frame
void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPickupComponent::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hey !"));
	IIInteractable* interactionObject = Cast<IIInteractable>(OtherActor);
	if (interactionObject != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Interaction !"));
		//interactionObject->InteractOverlap();
		listInteractables.Add(OtherActor);
	}
}

void UPickupComponent::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IIInteractable* interactionObject = Cast<IIInteractable>(OtherActor);
	if (interactionObject != nullptr)
	{
		//interactionObject->InteractEnd();
		listInteractables.Remove(OtherActor);
	}
}

IIInteractable* UPickupComponent::InteractWithFirstObject()//AIIM_ExoCharacter* user)
{
	/*if (listInteractables == nullptr)
		return;*/
	if (listInteractables.Num() == 0)
		return nullptr;
	IIInteractable* interactionObject = Cast<IIInteractable>(listInteractables[0]);
	return interactionObject;
	//interactionObject->Interact(user);
}

void UPickupComponent::RemoveObjectFromList()
{
	//listInteractables.RemoveAt(0);
}

