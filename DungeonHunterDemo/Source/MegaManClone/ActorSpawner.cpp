// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawner.h"
#include "AIEnemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AActorSpawner::AActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnPointComponent = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
	SpawnPointComponent->SetupAttachment(RootComponent);
	SpawnPointComponent->ArrowSize = 1.f;
	SpawnPointComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	CollisionBox->SetupAttachment(SpawnPointComponent);
	CollisionBox->SetCollisionProfileName("OverlapAllDynamic");
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AActorSpawner::OnTriggerOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AActorSpawner::EndTriggerOverlap);
	

	bSpawned = false;
}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AActorSpawner::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UBoxComponent* MeeleBox = Cast<UBoxComponent>(OtherComp);
	
	if (!MeeleBox)
	{
		SpawnActor();
	}
}

void AActorSpawner::EndTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bSpawned = false;
}

void AActorSpawner::SpawnActor()
{
	if (!bSpawned)
	{
		if (EnemyTemplate != nullptr)
		{
			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				FActorSpawnParameters SpawnParams;

				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


				FTransform SpawnTransform = SpawnPointComponent->GetComponentTransform();
				bSpawned = true;
				

				AAIEnemy* SpawnedActor = World->SpawnActor<AAIEnemy>(EnemyTemplate, SpawnTransform, SpawnParams = FActorSpawnParameters());
				SpawnedActor->SetPatrolPoints(FirstPatrolPoint , SecondPatrolPoint);
				
			}

		}
	}

}

