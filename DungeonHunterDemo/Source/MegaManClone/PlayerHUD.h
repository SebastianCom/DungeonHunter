// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANCLONE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
    APlayerHUD();

    UPROPERTY(EditAnywhere, Category = "Game Hud")
        TSubclassOf<class UUserWidget> PlayerGuiClass;

    UPROPERTY()
        class UUserWidget* PlayerGui;

protected:
    virtual void BeginPlay() override;
	
};
