// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

APlayerHUD::APlayerHUD()
{

}

void APlayerHUD::BeginPlay()
{
    PlayerGui = CreateWidget<UUserWidget>(GetGameInstance(), PlayerGuiClass);
    PlayerGui->AddToViewport();

}