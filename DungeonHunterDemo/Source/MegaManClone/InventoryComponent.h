// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UENUM(BlueprintType)
enum class EInventoryDirection : uint8 {

	NEXT = 0  UMETA(DisplayName = "NEXT"),
	PREV = 1  UMETA(DisplayName = "PREV")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGAMANCLONE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	void AddToInventory(class APickUpActor* pickup);

	int GetInventoryCount();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual void NextInventoryItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		virtual void PreviousInventoryItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		class APickUpActor* GetCurrentInventory() const;

	void ResetCurrentInventory();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class AGamePlay1Character* CharacterOwner;

	/** Item that is currently equipped */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		class APickUpActor* CurrentInventoryItem;

	void EquipNewInventoryItem(APickUpActor* NewItem);

	void SelectInventory(EInventoryDirection dir);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere)
		TArray< class APickUpActor* > Inventory;
		
};
