// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "HealthPickUp.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANCLONE_API AHealthPickUp : public APickUpActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
public:
	AHealthPickUp();
	UPROPERTY(EditDefaultsOnly, Category = AmmoCount)
		uint32 HealthPickUpAmount;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UHoverComponent* HoverComponent;
	
};
