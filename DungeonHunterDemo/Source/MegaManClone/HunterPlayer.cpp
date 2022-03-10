// Fill out your copyright notice in the Description page of Project Settings.


#include "HunterPlayer.h"
#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

#include "Kismet/GameplayStatics.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "InventoryComponent.h"
#include "PickupActor.h"
#include "Weapon.h"

#include "Components/AudioComponent.h"

#include "AmmoPickup.h"

#include "HealthComponent.h"
#include "HealthPickUp.h"
#include "CheckPoint.h"
#include "AIEnemy.h"
#include "EndPoint.h"



AHunterPlayer::AHunterPlayer()
{

	//QUESTIONABLE
	// Make sure to only use Yaw from controller and ignore rest of rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	//Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.0f, 50.f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetUsingAbsoluteRotation(true);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->bAutoActivate = true;

	//Keep character on 2d plane
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.f, 0.f));

	MeeleBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MeeleBox"));
	MeeleBox->SetupAttachment(GetSprite());
	MeeleBox->SetCollisionProfileName("OverlapAll");
	MeeleBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsAttacking = false;
	bIsAlive = true;
	bCanAttack = true;
	bCanBeDamaged = true;
	WeaponDelay = 0.2f;
	LastAttackAnimation = ShootAnimation;
	DmgDealt = false;
	MeeleDmg = 1;

	CharacterScene = CreateDefaultSubobject<USceneComponent>("PlayerScene");
	CharacterScene->SetupAttachment(GetSprite());

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");

	PickupSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	PickupSphereComponent->SetupAttachment(RootComponent);
	PickupSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupSphereComponent->SetCollisionProfileName("OverlapAll");

	PickupSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHunterPlayer::OnPickUpOverlap);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	AudioComponent->SetupAttachment(RootComponent);

	bOnFloor = true;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHunterPlayer::OnHit);

	Health = CreateDefaultSubobject<UHealthComponent>("Health");

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("Noise Emitter");
}

void AHunterPlayer::MakeSomeNoise()
{
	MakeNoise(1.0f, this, GetActorLocation());
}

void AHunterPlayer::BeginPlay()
{
	Super::BeginPlay();

	MeeleBox->OnComponentBeginOverlap.AddDynamic(this, &AHunterPlayer::CombatOverlapBegin);
	MeeleBox->OnComponentEndOverlap.AddDynamic(this, &AHunterPlayer::CombatOverlapEnd);

	//Debugging
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (controller)
	{
		//FString command = "show collision";
		//controller->ConsoleCommand(command, true);
		controller->bShowMouseCursor = true;
		controller->bEnableClickEvents = true;
		controller->bEnableMouseOverEvents = true;
	}
	Health->SetCheckPoint(GetActorLocation());
}



void AHunterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//float health = Health->GetHealth();
	//GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("Health: %f"), health, false));

	Health->CheckAlive(this);

	UpdateCharacter();
	
}

void AHunterPlayer::MoveRight(float value)
{
	if (bIsAlive && !bIsAttacking && bCanBeDamaged)
	{
		AddMovementInput(FVector(1.f, 0.f, 0.f), value * .4f);
		MakeSomeNoise();
	}

}

void AHunterPlayer::UpdateAnimation()
{
	bool bIsFalling = GetMovementComponent()->IsFalling();
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqruared = PlayerVelocity.SizeSquared();

	if (bIsAlive)
	{
		if (!bIsFalling)
		{
			if (!bIsAttacking)
			{
				if (bCanBeDamaged)
				{
					UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqruared > 0.f) ? RunAnimation : IdleAnimation;

					if (GetSprite()->GetFlipbook() != DesiredAnimation)
					{
						GetSprite()->SetFlipbook(DesiredAnimation);
					}


				}
			}
			else
			{

				GetSprite()->SetFlipbook(LastAttackAnimation);
			}
		}
		else
		{
			if (GetSprite()->GetFlipbook() != JumpAnimation)
				GetSprite()->SetFlipbook(JumpAnimation);
		}
	}
	else
	{
		GetSprite()->SetFlipbook(DeathAnimation);
	}

}


void AHunterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveRight", this, &AHunterPlayer::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHunterPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AHunterPlayer::Attack);
	
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AHunterPlayer::PlayShootAnimation);

	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AHunterPlayer::PickupWeapon);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AHunterPlayer::NextWeapon);
	PlayerInputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AHunterPlayer::PrevWeapon);

}

void AHunterPlayer::UpdateCharacter()
{
	UpdateAnimation();
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;

	if (Controller != nullptr)
	{
		if (TravelDirection < 0.f)
		{
			Controller->SetControlRotation(FRotator(0.f, 180.f, 0.f));
		}
		else if (TravelDirection > 0.f)
		{
			Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));
		}
	}

	if (TravelDirection != 0 && bOnFloor)
	{
		if (!AudioComponent->IsPlaying())
		{
			AudioComponent->SetSound(WalkSound);
			AudioComponent->Play();
		}
	}

}

void AHunterPlayer::Jump()
{

	//Health->TakeDamage(5);

	if (!bIsAttacking && bOnFloor)
	{
		Super::Jump();
		AudioComponent->SetSound(JumpSound);
		AudioComponent->Play();

		bOnFloor = false;
	}
}

void AHunterPlayer::Attack()
{
	bool bIsFalling = GetMovementComponent()->IsFalling();
	//We have to make sure we add a delay between attacks, so players can't just hack and slash their way out
	if (!bIsFalling && bIsAlive)
	{
		if (!bIsAttacking && bCanAttack)
		{
			LastAttackAnimation = MeeleAnimation;
			AudioComponent->SetSound(MeeleSound);
			AudioComponent->Play();
			bIsAttacking = true;
			ActivateCollision(MeeleBox);
			bCanAttack = false;
			GetWorldTimerManager().SetTimer(timerHandle, this, &AHunterPlayer::EndAttack, 0.5f);
		}
	}
}

void AHunterPlayer::EndAttack()
{
	bIsAttacking = false;
	DeactivateCollision(MeeleBox);
	GetWorldTimerManager().SetTimer(attackDelayHandle, this, &AHunterPlayer::ResetWeaponDelay, WeaponDelay);

}

void AHunterPlayer::ResetWeaponDelay()
{
	bCanAttack = true;
}

void AHunterPlayer::Land()
{
	bOnFloor = true;
	AudioComponent->SetSound(LandSound);
    AudioComponent->Play();
}

void AHunterPlayer::Die()
{
	bIsAlive = false;
	AudioComponent->SetSound(HurtSound);
	AudioComponent->Play();
	if (!bIsAlive)
	{
		GetWorldTimerManager().SetTimer(DeathHandle, Health, &UHealthComponent::Repawn, 1.0f);
	}
}

void AHunterPlayer::Repawn()
{
	bIsAlive = true;
}

void AHunterPlayer::ActivateCollision(UBoxComponent* Comp)
{
	Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AHunterPlayer::DeactivateCollision(UBoxComponent* Comp)
{
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DmgDealt = false;
}

void AHunterPlayer::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AAIEnemy* Enemy = Cast<AAIEnemy>(OtherActor);

		if (Enemy != nullptr)
		{
			if (!DmgDealt)
			{
				Enemy->Health->TakeDamage(MeeleDmg);

				DmgDealt = true;

				//float TravelDirection = GetVelocity().X;

				//if (Controller != nullptr)
				//{
				//	if (TravelDirection < 0.f)
				//	{
				//		Enemy->SetActorLocation(Enemy->GetActorLocation() += FVector(-50, 0, 0));
				//	}
				//	else if (TravelDirection > 0.f)
				//	{
				//		Enemy->SetActorLocation(Enemy->GetActorLocation() += FVector(50, 0, 0));
				//	}
				//}

			}
		}
	}
}

void AHunterPlayer::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


void AHunterPlayer::NextWeapon()
{
	EquipFromInventory(EInventoryDirection::NEXT);
}

void AHunterPlayer::PrevWeapon()
{
	EquipFromInventory(EInventoryDirection::PREV);
}

void AHunterPlayer::PickupWeapon()
{
	TArray<AActor*> overlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(overlappingActors);
	
	for (int i = 0; i < overlappingActors.Num(); i++)
	{
		if (overlappingActors[i]->ActorHasTag("Pickup"))
		{      	
			InventoryComponent->AddToInventory(Cast<APickUpActor>(overlappingActors[i]));
			CurrentWeapon = Cast<AWeapon>(overlappingActors[i]);
			if (CurrentWeapon != nullptr)
			{
				//if (AudioComponent->IsPlaying() == false)
				{
					AudioComponent->SetSound(PistolEquipSound);
					AudioComponent->Play();
				}
			}
		}
	}

}

void AHunterPlayer::PlayShootAnimation()
{
	if (CurrentWeapon)
	{
		if (bCanAttack)
		{
			if (CurrentWeapon->CurrentAmmo > 0)
			{
				LastAttackAnimation = ShootAnimation;
				bIsAttacking = true;
				bCanAttack = false;
				GetWorldTimerManager().SetTimer(ShootDelayHandle, this, &AHunterPlayer::Shoot, 0.8f);
			}
			else
			{
				AudioComponent->SetSound(OutOfAmmo);
				AudioComponent->Play();
			}
		}
	}
}

void AHunterPlayer::EndGame()
{
	Destroy();
}

bool AHunterPlayer::HasWeapon()
{
	if (CurrentWeapon)
		return true;
	else
		return false;
}

int AHunterPlayer::GetCurrentAmmo()
{
	if (CurrentWeapon)
		return CurrentWeapon->CurrentAmmo;
	else
		return 0;
}

int AHunterPlayer::GetMaxAmmo()
{
	if (CurrentWeapon)
		return CurrentWeapon->MaximumAmmo;
	else return 0;
}

float AHunterPlayer::GetHealth()
{
	float HealthNumber = Health->HealthAmount;
	return HealthNumber;
}

float AHunterPlayer::GetMaxHealth()
{
	float MaxHealthNumber = Health->MaxHealth;
	return MaxHealthNumber;
}

void AHunterPlayer::Shoot()
{

	if (CurrentWeapon)
	{
			CurrentWeapon->Fire();
			LastAttackAnimation = ShootAnimation_2;
			GetWorldTimerManager().SetTimer(ShootDelayHandle, this, &AHunterPlayer::StopShoot, 0.7f);
	}
}

void AHunterPlayer::StopShoot()
{
	bIsAttacking = false;
	bCanAttack = true;
}



void AHunterPlayer::EquipFromInventory(EInventoryDirection dir)
{

	if (CurrentWeapon != nullptr)
	{
		if (!bIsAttacking)
		{
			CurrentWeapon->UnEquip();
		}
	}

	if (dir == EInventoryDirection::NEXT)
	{
		if (!bIsAttacking)
		{
			InventoryComponent->NextInventoryItem();
		}
	}
	else
	{
		if (!bIsAttacking)
		{
			InventoryComponent->PreviousInventoryItem();
		}
	}

	CurrentWeapon = Cast<AWeapon>(InventoryComponent->GetCurrentInventory());
	if (CurrentWeapon != nullptr)
	{

		CurrentWeapon->Equip(this);		
	}

	if(CurrentWeapon != nullptr)
	{
		if (AudioComponent->IsPlaying() == false && !bIsAttacking)
		{
			AudioComponent->SetSound(PistolEquipSound);
			AudioComponent->Play();
		}
	}

}

void AHunterPlayer::OnPickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Ammo"))
		{
			AAmmoPickUp* Ammo = Cast<AAmmoPickUp>(OtherActor);

			if (Ammo != nullptr)
			{
				if (CurrentWeapon != nullptr)
				{	
					CurrentWeapon->CurrentAmmo += Ammo->AmmoCount;
					Ammo->Destroy();
					if (CurrentWeapon->CurrentAmmo > CurrentWeapon->MaximumAmmo)
					{
						CurrentWeapon->CurrentAmmo = CurrentWeapon->MaximumAmmo;
					}
				}
			}
		}
		if (OtherActor->ActorHasTag("Health"))
		{
			AHealthPickUp* HealthPickUp = Cast<AHealthPickUp>(OtherActor);

			if (HealthPickUp != nullptr)
			{
				Health->ReplenHealth(HealthPickUp->HealthPickUpAmount);
				HealthPickUp->Destroy();
			}
		}
		if (OtherActor->ActorHasTag("CheckPoint"))
		{
			ACheckPoint* CheckPoint = Cast<ACheckPoint>(OtherActor);

			if (CheckPoint != nullptr)
			{
				Health->SetCheckPoint(CheckPoint->GetActorLocation());
				CheckPoint->Trigger();
			}
		}
		if (OtherActor->ActorHasTag("EndPoint"))
		{
			AEndPoint* EndPoint = Cast<AEndPoint>(OtherActor);

			if (EndPoint != nullptr)
			{
				EndPoint->Trigger();
				GetWorldTimerManager().SetTimer(EndHandle, this, &AHunterPlayer::EndGame, 2.8f);
			}
		}
		if (OtherActor->ActorHasTag("DeathBox"))
		{
			Health->TakeDamage(Health->MaxHealth);
		}
	}
}

void AHunterPlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->ActorHasTag("Floor"))
		{
			Land();
		}
		else // This is for the test level only, to show to difference of floor tile vs default floor.
		{
			bOnFloor = true;
		}
	}
}

