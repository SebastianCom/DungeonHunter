// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include <Components/SphereComponent.h>
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "PaperSpriteComponent.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
    Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Sprite->SetCollisionProfileName("NoCollision");

    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereCollision->SetSphereRadius(24.f);
    RootComponent = SphereCollision;
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereCollision->SetCollisionProfileName("OverlapAll");
    SphereCollision->SetGenerateOverlapEvents(true);

    SphereCollision->SetSimulatePhysics(false);

    Sprite->SetupAttachment(RootComponent);
    
    SphereCollision->GetBodyInstance()->bLockRotation = true;
    SphereCollision->GetBodyInstance()->bLockXRotation = true;
    SphereCollision->GetBodyInstance()->bLockYRotation = true;
    SphereCollision->GetBodyInstance()->bLockZRotation = true;
    SphereCollision->GetBodyInstance()->bLockYTranslation = true;

    Tags.Add("Pickup");
    
}

void APickUpActor::Disable()
{
    if (Sprite)
    {
        this->SetActorHiddenInGame(true);
        SphereCollision->SetActive(false);
        SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        //so i do not see the equiped gun. My character sprite has a gun already
        Sprite = nullptr;
    }
}

void APickUpActor::Enable()
{
    if (Sprite)
    {
        this->SetActorHiddenInGame(false);
        SphereCollision->SetActive(true);
        SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APickUpActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();


    //IF WBP_PickupIcon IS NOT nullptr
    if (GunIcon != nullptr)
    {
        //ASSIGN PickupIcon to the return value of CreateWidget<UUserWidget>(GetWorld(), WBP_PickupIcon)
        PickupIcon = CreateWidget<UUserWidget>(GetWorld(), GunIcon);

    }
}

void APickUpActor::DisplayInventoryIcon(bool visible)
{
    if (PickupIcon->IsValidLowLevel())
    {
        if (visible)
            PickupIcon->AddToViewport();
        else
            PickupIcon->RemoveFromViewport();
    }
}

