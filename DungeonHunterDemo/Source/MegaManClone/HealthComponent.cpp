// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "HunterPlayer.h"
#include "AIEnemy.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsAlive = true;
	//LastCheckPoint = FVector(50, 0, 50);

	// ...
}


void UHealthComponent::TakeDamage(int dmg)
{
	HealthAmount -= dmg;

}

void UHealthComponent::ReplenHealth(int hlth)
{
	HealthAmount += hlth;

	if (HealthAmount > MaxHealth)
	{
		HealthAmount = MaxHealth;
	}
	

}

void UHealthComponent::CheckAlive(AActor* actor)
{
	if (HealthAmount <= 0 && bIsAlive)
	{
		OwningActor = actor;
		bIsAlive = false;
		AHunterPlayer* OwningPlayer = Cast<AHunterPlayer>(GetOwner());
		if (OwningPlayer)
		{
			OwningPlayer->Die();
		}
		
		AAIEnemy* OwningEnemy = Cast<AAIEnemy>(GetOwner());
		if (OwningEnemy)
		{
			OwningEnemy->Die();
		}
	}
}

void UHealthComponent::Repawn()
{
	OwningActor->SetActorLocation(LastCheckPoint);
	bIsAlive = true;
	HealthAmount = RespawnHealth;
	AHunterPlayer* OwningPlayer = Cast<AHunterPlayer>(GetOwner());
	if (OwningPlayer)
	{
		OwningPlayer->Repawn();
	}
	
	AAIEnemy* OwningEnemy = Cast<AAIEnemy>(GetOwner());
	if (OwningEnemy)
	{
		OwningEnemy->Destroy();
	}
}

void UHealthComponent::SetCheckPoint(FVector cp)
{
	LastCheckPoint = cp;
}

int32 UHealthComponent::GetHealth()
{
	return HealthAmount;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

