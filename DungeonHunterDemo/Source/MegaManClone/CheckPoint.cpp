// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"



// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Empty();
	Tags.Add("CheckPoint");

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Checkpoint Trigger"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionProfileName("OverlapAll");

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Checkpoint Sprite"));
	Sprite->SetupAttachment(BoxCollision);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sprite->SetCollisionProfileName("NoCollision");

	TriggeredSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Checkpoint Sprite2"));
	TriggeredSprite->SetupAttachment(Sprite);
	TriggeredSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggeredSprite->SetCollisionProfileName("NoCollision");
	TriggeredSprite->SetVisibility(false);


}

void ACheckPoint::Trigger()
{
	Sprite->SetVisibility(false);
	TriggeredSprite->SetVisibility(true);
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

