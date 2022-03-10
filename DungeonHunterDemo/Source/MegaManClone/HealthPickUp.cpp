// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickUp.h"
#include "HoverComponent.h"

AHealthPickUp::AHealthPickUp()
{
    Tags.Empty();
    Tags.Add("Health");

    HoverComponent = CreateDefaultSubobject<UHoverComponent>("Hover");
    HoverComponent->SetupAttachment(RootComponent);
}

void AHealthPickUp::BeginPlay()
{
    Super::BeginPlay();
    HoverComponent->initHover(this);
}

void AHealthPickUp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
