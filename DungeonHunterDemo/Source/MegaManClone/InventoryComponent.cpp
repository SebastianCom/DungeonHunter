// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "PickupActor.h"
#include "HunterPlayer.h"
#include "PaperSpriteComponent.h"



// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


void UInventoryComponent::AddToInventory(APickUpActor* pickup)
{
	Inventory.AddUnique(pickup);
	pickup->Disable();
	EquipNewInventoryItem(pickup);
}

int UInventoryComponent::GetInventoryCount()
{
	return Inventory.Num();
}

void UInventoryComponent::NextInventoryItem()
{
	SelectInventory(EInventoryDirection::NEXT);
}

void UInventoryComponent::PreviousInventoryItem()
{
	SelectInventory(EInventoryDirection::PREV);
}

APickUpActor* UInventoryComponent::GetCurrentInventory() const
{
	return CurrentInventoryItem;
}

void UInventoryComponent::ResetCurrentInventory()
{
	CurrentInventoryItem = nullptr;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInventoryComponent::EquipNewInventoryItem(APickUpActor* NewItem)
{
	if (CurrentInventoryItem)
	{
		CurrentInventoryItem->DisplayInventoryIcon(false);
		CurrentInventoryItem->Disable();
		CurrentInventoryItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (NewItem)
	{
		NewItem->DisplayInventoryIcon(true);
		NewItem->Enable();
		AHunterPlayer* OwningActor = Cast<AHunterPlayer>(GetOwner());
		NewItem->AttachToComponent(OwningActor->CharacterScene, FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
	}

	CurrentInventoryItem = NewItem;


}

void UInventoryComponent::SelectInventory(EInventoryDirection dir)
{

	if (Inventory.Num() == 0)
	{
		return;
	}

	APickUpActor* InventoryItemToEquip = nullptr;
	int32 Index = (dir == EInventoryDirection::NEXT ? 0 : Inventory.Num() - 1);

	if (CurrentInventoryItem != nullptr)
	{

		Inventory.Find(CurrentInventoryItem, Index);


		dir == EInventoryDirection::NEXT ? Index += 1 : Index -= 1; 

	}

	if (Index < Inventory.Num() && Index > -1)
	{
		InventoryItemToEquip = Inventory[Index];
	}

	EquipNewInventoryItem(InventoryItemToEquip);

}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

