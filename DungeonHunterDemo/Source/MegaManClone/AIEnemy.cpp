// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemy.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "PaperFlipbookComponent.h"
#include "NavigationSystem.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/AISense_Sight.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "HunterPlayer.h"
#include "HealthComponent.h"

AAIEnemy::AAIEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing");


	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIEnemy::OnPawnSeen);

	PawnSensingComp->OnHearNoise.AddDynamic(this, &AAIEnemy::OnNoiseHeard);

	GuardState = EAIState::Idle;
	bIsAttacking = false;
	bCanAttack = true;
	AttackDelay = 2.0f;
	DmgAmount = 2;
	DmgDealt = false;
	Punching = false;

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	AttackSphere->SetupAttachment(RootComponent);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AttackSphere->SetCollisionProfileName("OverlapAll");

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AAIEnemy::OnAttackOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AAIEnemy::EndAttackOverlap);

	MeeleBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeeleBox"));
	MeeleBox->SetupAttachment(GetSprite());
	MeeleBox->SetCollisionProfileName("OverlapAll");
	MeeleBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Health = CreateDefaultSubobject<UHealthComponent>("Health");

	bIsAlive = true;
}

void AAIEnemy::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		AHunterPlayer* Player = Cast<AHunterPlayer>(OtherActor);

		if (Player != nullptr)
		{
			if (bCanAttack)
			{
				Punching = true;
				//Attack();
			}
		}
		
	}
}

void AAIEnemy::EndAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Punching = false;
}

void AAIEnemy::UpdateAnimation()
{

	const FVector EnemyVelocity = GetVelocity();
	const float EnemySpeedSqruared = EnemyVelocity.SizeSquared();
	if (bIsAlive)
	{
		if (!bIsAttacking)
		{
			UPaperFlipbook* DesiredAnimation = (EnemySpeedSqruared > 0.f) ? RunAnimation : IdleAnimation;

			if (GetSprite()->GetFlipbook() != DesiredAnimation)
			{
				GetSprite()->SetFlipbook(DesiredAnimation);
			}
		}
		else if (bCanAttack)
		{

			GetSprite()->SetFlipbook(MeeleAnimation);
		}
	}
	else
	{
		GetSprite()->SetFlipbook(DeathAnimation);
	}
}

void AAIEnemy::Attack()
{
	bIsAttacking = true;
	ActivateCollision(MeeleBox);
	GetWorldTimerManager().SetTimer(timerHandle, this, &AAIEnemy::EndAttack, 0.7f);
}

void AAIEnemy::EndAttack()
{
	bIsAttacking = false;
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(attackDelayHandle, this, &AAIEnemy::ResetWeaponDelay, AttackDelay);
}

void AAIEnemy::ResetWeaponDelay()
{
	bCanAttack = true;
}

void AAIEnemy::ActivateCollision(UBoxComponent* Comp)
{
	Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAIEnemy::DeactivateCollision(UBoxComponent* Comp)
{
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DmgDealt = false;
}

void AAIEnemy::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AHunterPlayer* Player = Cast<AHunterPlayer>(OtherActor);

		if (Player != nullptr)
		{

			Player->Health->TakeDamage(DmgAmount);
			DeactivateCollision(MeeleBox);
			DmgDealt = true;

			float TravelDirection = GetVelocity().X;

			//if (Controller != nullptr)
			//{
			//	if (TravelDirection < 0.f)
			//	{
			//		Player->SetActorLocation(Player->GetActorLocation() += FVector(-50, 0, 0));
			//	}
			//	else if (TravelDirection > 0.f)
			//	{
			//		Player->SetActorLocation(Player->GetActorLocation() += FVector(50, 0, 0));
			//	}
			//}
				
		}
	}
}

void AAIEnemy::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AAIEnemy::Die()
{
	bIsAlive = false;

	if (!bIsAlive)
	{
		GetWorldTimerManager().SetTimer(DeathHandle, Health, &UHealthComponent::Repawn, 1.0f);
	}
}

void AAIEnemy::ShouldPunch()
{
	if (bIsAlive)
	{
		if (!bIsAttacking)
		{
			if (bCanAttack)
			{
				if (Punching)
				{
					Attack();
				}
			}
		}
	}
}

void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();

	MeeleBox->OnComponentBeginOverlap.AddDynamic(this, &AAIEnemy::CombatOverlapBegin);
	MeeleBox->OnComponentEndOverlap.AddDynamic(this, &AAIEnemy::CombatOverlapEnd);

	pController = GetController();

	OriginalRotation = GetActorRotation();

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AAIEnemy::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;
}

void AAIEnemy::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	TargetActor = SeenPawn;

	//DrawDebugSphere(GetWorld(),
	//	SeenPawn->GetActorLocation(),
	//	32.0f,
	//	12,
	//	FColor::Red,
	//	false,
	//	10.0f);

	SetGuardState(EAIState::Alerted);

	if (pController != nullptr)
	{
		pController->StopMovement();
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AAIEnemy::ResetAIState, 3.0f);
	}

}

void AAIEnemy::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	/*DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);*/

	FVector Direction = Location - GetActorLocation();

	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AAIEnemy::ResetOrientation, 3.0f);

	SetGuardState(EAIState::Suspicious);

	if (pController != nullptr)
	{
		pController->StopMovement();
	}
	//ENDIF
}

void AAIEnemy::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);

	if (bPatrol)
	{
		ResetAIState();
	}
	else
	{
		SetGuardState(EAIState::Idle);
	}
}

void AAIEnemy::ResetAIState()
{
	TargetActor = nullptr;
	SetGuardState(EAIState::Idle);

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAnimation();
	Health->CheckAlive(this);

	ShouldPunch();

	if (bIsAlive)
	{
		if (CurrentPatrolPoint && TargetActor == nullptr)
		{
			FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
			float DistanceToGoal = Delta.Size();

			if (DistanceToGoal < 100)
			{
				MoveToNextPatrolPoint();
			}
		}


		if (TargetActor != nullptr)
		{
			if (pController)
			{
				if (!bIsAttacking)
				{
					UAIBlueprintHelperLibrary::SimpleMoveToActor(pController, TargetActor);
				}
			}
		}
	}
}

void AAIEnemy::SetPatrolPoints(AActor* first, AActor* second)
{
	FirstPatrolPoint = first;
	SecondPatrolPoint = second;
	MoveToNextPatrolPoint();
}

void AAIEnemy::MoveToNextPatrolPoint()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "MoveToNextPatrolPoint - ");
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	if (pController)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(pController, CurrentPatrolPoint);
	}
}
