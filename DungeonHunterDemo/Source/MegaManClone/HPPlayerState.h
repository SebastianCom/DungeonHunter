// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANCLONE_API AHPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AHPPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Health")
		int GetHealth();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
		int GetCurrentAmmo();

	UFUNCTION(BlueprintCallable, Category = "Health")
		int GetMaxAmmo();

	int Health;
	int Ammo;
	int MaxAmmo;
	
};
