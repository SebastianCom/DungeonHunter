// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PaperSprite.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AIEnemy.h"
#include "HealthComponent.h"
#include "HunterPlayer.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    /** sphere collision  */
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetSphereRadius(4.f);
    SphereComponent->BodyInstance.SetCollisionProfileName("BlockAll");
    SphereComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

    SphereComponent->GetBodyInstance()->bLockRotation = true;
    SphereComponent->GetBodyInstance()->bLockXRotation = true;
    SphereComponent->GetBodyInstance()->bLockYRotation = true;
    SphereComponent->GetBodyInstance()->bLockZRotation = true;
    SphereComponent->GetBodyInstance()->bLockYTranslation = true;
    RootComponent = SphereComponent;

    /** paper sprite component  */
    ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperFlipbookComponent"));
    ProjectileSprite->SetupAttachment(RootComponent);
    ProjectileSprite->SetRelativeLocation(FVector(-3.f, 0.f, 0.f));

    /** ProjectileMovementComponent  */
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->UpdatedComponent = SphereComponent;
    ProjectileMovementComponent->InitialSpeed = InitialSpeed;
    ProjectileMovementComponent->MaxSpeed = MaximumSpeed;

    ProjectileMovementComponent->bRotationFollowsVelocity = false;
    ProjectileMovementComponent->bShouldBounce = true;

    InitialLifeSpan = 5.f;

}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::SetFireDirection(FVector dir)
{
    ProjectileMovementComponent->Velocity = InitialSpeed * dir;
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if (OtherActor)
    {
        AAIEnemy* Enemy = Cast<AAIEnemy>(OtherActor);

        if (Enemy != nullptr)
        {
            Enemy->Health->TakeDamage(ShotDmg);

            Destroy();
        }
    }
    if(OtherComp)
    {
        AHunterPlayer* Player = Cast<AHunterPlayer>(OtherComp);
        if (Player == nullptr)
        {
            Destroy();
        }
    }
}

