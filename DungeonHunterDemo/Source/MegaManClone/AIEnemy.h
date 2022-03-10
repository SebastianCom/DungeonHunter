// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericTeamAgentInterface.h"
#include "PaperCharacter.h"
#include "Modules/ModuleManager.h"
#include "AIEnemy.generated.h"




UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	Alerted
};

class UPawnSensingComponent;

UCLASS()
class MEGAMANCLONE_API AAIEnemy : public APaperCharacter
{
	GENERATED_BODY()

public:

	AAIEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* MeeleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* DeathAnimation;

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class USphereComponent* AttackSphere;


	UFUNCTION()
		void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateAnimation();

	UFUNCTION()
	void Attack();

	void EndAttack();
	void ResetWeaponDelay();

	bool bIsAttacking;
	bool bCanAttack;
	float AttackDelay;
	bool DmgDealt;
	int DmgAmount;

	FTimerHandle timerHandle;
	FTimerHandle attackDelayHandle;
	FTimerHandle DeathHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class UBoxComponent* MeeleBox;

	void ActivateCollision(UBoxComponent* Comp);
	void DeactivateCollision(UBoxComponent* Comp);

	UFUNCTION()
		void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* Health;

	void Die();
	bool bIsAlive;

	bool Punching;
	void ShouldPunch();

protected:
	// Called when the game starts or when spawned


	FRotator OriginalRotation;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
		void ResetOrientation();

	UFUNCTION()
		void ResetAIState();

	FTimerHandle TimerHandle_ResetOrientation;


public:
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPatrolPoints(AActor* first, AActor* second);



protected:

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* SecondPatrolPoint;


	UFUNCTION()
		void MoveToNextPatrolPoint();

	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatrol;



	AActor* CurrentPatrolPoint;


private:

	AActor* TargetActor;
	FVector HeardLocation;
	AController* pController;
	
	
};
