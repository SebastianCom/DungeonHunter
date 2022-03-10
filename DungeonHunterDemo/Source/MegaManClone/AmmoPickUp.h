// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "AmmoPickUp.generated.h"


/**
 * 
 */
UCLASS()
class MEGAMANCLONE_API AAmmoPickUp : public APickUpActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
public:
	AAmmoPickUp();
	UPROPERTY(EditDefaultsOnly, Category = AmmoCount)
		uint32 AmmoCount;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UHoverComponent* HoverComponent;
};
