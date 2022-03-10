// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatfrom.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMovingPlatfrom::AMovingPlatfrom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Sprite = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite");
    Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Sprite->SetCollisionProfileName("NoCollision");

    Sprite2 = CreateDefaultSubobject<UPaperSpriteComponent>("Sprite2");
    Sprite2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Sprite2->SetCollisionProfileName("NoCollision");

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxCollision;
    BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    BoxCollision->SetCollisionProfileName("BlockAll");
    BoxCollision->SetGenerateOverlapEvents(true);

    BoxCollision->SetSimulatePhysics(false);

    Sprite->SetupAttachment(RootComponent);
    Sprite2->SetupAttachment(RootComponent);

    BoxCollision->GetBodyInstance()->bLockRotation = true;
    BoxCollision->GetBodyInstance()->bLockXRotation = true;
    BoxCollision->GetBodyInstance()->bLockYRotation = true;
    BoxCollision->GetBodyInstance()->bLockZRotation = true;
    //BoxCollision->GetBodyInstance()->bLockYTranslation = true;

    Tags.Add("Floor");
    
    MovingToFirst = true;
}

void AMovingPlatfrom::MoveToNextPoint()
{
    if (NextPoint == nullptr || CurrentPoint == SecondPoint)
    {
        NextPoint = FirstPoint;
    }
    else if(CurrentPoint == FirstPoint)
    {
        NextPoint = SecondPoint; 
    }
}

void AMovingPlatfrom::MovePlatform(float DeltaTime)
{
    if (CurrentPoint != NextPoint)
    {
       SetActorLocation( UKismetMathLibrary::VInterpTo(GetActorLocation(), NextPoint->GetActorLocation(), DeltaTime, .5));
    }
}

// Called when the game starts or when spawned
void AMovingPlatfrom::BeginPlay()
{
	Super::BeginPlay();
    MoveToNextPoint();
}

// Called every frame
void AMovingPlatfrom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    

     FVector Delta = GetActorLocation() - NextPoint->GetActorLocation();
     float DistanceToGoal = Delta.Size();

     if (DistanceToGoal < 10)
     {
         CurrentPoint = NextPoint;
         MoveToNextPoint();
     }

     MovePlatform(DeltaTime);

}

