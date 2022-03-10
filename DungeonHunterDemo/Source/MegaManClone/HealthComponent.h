// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGAMANCLONE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditDefaultsOnly, Category = HealthAmount)
		int32 HealthAmount;

	UPROPERTY(EditDefaultsOnly, Category = HealthAmount)
		int32 RespawnHealth;

	UPROPERTY(EditDefaultsOnly, Category = HealthAmount)
		int32 MaxHealth;


	void TakeDamage(int dmg);
	void ReplenHealth(int hlth);
	bool bIsAlive;
	void CheckAlive(AActor* actor);
	void Repawn();

	void SetCheckPoint(FVector cp);
	
	AActor* OwningActor;

	FVector LastCheckPoint;


	int32 GetHealth();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
