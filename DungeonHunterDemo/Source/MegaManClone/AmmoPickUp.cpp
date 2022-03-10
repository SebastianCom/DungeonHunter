// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickUp.h"
#include "HoverComponent.h"

AAmmoPickUp::AAmmoPickUp()
{
    Tags.Empty();
    Tags.Add("Ammo");

    HoverComponent = CreateDefaultSubobject<UHoverComponent>("Hover");
    HoverComponent->SetupAttachment(RootComponent);
}

void AAmmoPickUp::BeginPlay()
{
    Super::BeginPlay();
    HoverComponent->initHover(this);
}

void AAmmoPickUp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
