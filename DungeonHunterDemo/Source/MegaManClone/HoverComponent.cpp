// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverComponent.h"
#include "PaperSpriteComponent.h"


// Sets default values for this component's properties
UHoverComponent::UHoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//ShapeToHover = CreateDefaultSubobject<UPrimitiveComponent>("Hover");
	// ...
}


void UHoverComponent::initHover(APickUpActor* shape)
{
	ShapeToHover = shape;
	StartingLocation = ShapeToHover->GetActorLocation();
	bMovingUp = true;
}

// Called when the game starts
void UHoverComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShapeToHover)
	{
		FVector CurrentLocation = ShapeToHover->GetActorLocation();
		FVector HoverHeight = FVector(0, 0, 10);
		

		if (CurrentLocation.Z < (StartingLocation.Z + HoverHeight.Z) && bMovingUp == true)
			ShapeToHover->SetActorLocation(CurrentLocation += (FVector(0,0,7) * DeltaTime));
		else
		{
			bMovingUp = false;
			if (CurrentLocation.Z > StartingLocation.Z)
				ShapeToHover->SetActorLocation(CurrentLocation -= (FVector(0, 0, 7) * DeltaTime));
			else
				bMovingUp = true;
		}
	}
}

