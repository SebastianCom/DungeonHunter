// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Empty();
	Tags.Add("Floor");

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxCollision->SetCollisionProfileName("BlockAll");

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Endpoint Sprite"));
	Sprite->SetupAttachment(BoxCollision);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sprite->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

