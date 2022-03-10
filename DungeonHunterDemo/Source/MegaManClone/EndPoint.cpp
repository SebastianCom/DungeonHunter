// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AEndPoint::AEndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	Tags.Empty();
	Tags.Add("EndPoint");

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Endpoint Trigger"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionProfileName("OverlapAll");

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Endpoint Sprite"));
	Sprite->SetupAttachment(BoxCollision);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sprite->SetCollisionProfileName("NoCollision");

	TriggeredSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Endpoint Sprite2"));
	TriggeredSprite->SetupAttachment(Sprite);
	TriggeredSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggeredSprite->SetCollisionProfileName("NoCollision");
	TriggeredSprite->SetVisibility(false);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	AudioComponent->SetupAttachment(RootComponent);
}

void AEndPoint::Trigger()
{
	Sprite->SetVisibility(false);
	TriggeredSprite->SetVisibility(true);
	AudioComponent->SetSound(EndSound);
	AudioComponent->Play();
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

