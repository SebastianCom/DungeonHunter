// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/SphereComponent.h>
#include <Components/ArrowComponent.h>
#include "Net/UnrealNetwork.h"
#include "HunterPlayer.h"
#include "Projectile.h"
#include "PaperSpriteComponent.h"
#include "HPPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"

AWeapon::AWeapon() : Super()
{
	FireRate = 10.0f;
	MaximumAmmo = 6;
	CurrentAmmo = 0;
	//CurrentAmmo = 20;
	Muzzle = CreateDefaultSubobject<UArrowComponent>("Muzzle");
	Muzzle->SetupAttachment(RootComponent);


	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	AudioComponent->SetupAttachment(RootComponent);

}

void AWeapon::Equip(AHunterPlayer* Character)
{

	//ENABLE this Weapon
	Enable();

	//SET owner of this weapon to Character. There is a function in AActor that does this
	SetOwner(Character);

	//ATTACH weapon to the Character's PlayerSprite at WeaponSocket.
	AttachToComponent(Character->CharacterScene, FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
	

	
}

void AWeapon::UnEquip()
{
	Disable();
	SetOwner(nullptr);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeapon::Fire()
{
	if (CurrentAmmo > 0)
	{
		if (AudioComponent->IsPlaying() == false)
		{
			AudioComponent->Play();
		}

		SpawnProjectile();
	}
}

FVector AWeapon::GetMuzzleLocation() const
{
	return Muzzle->GetComponentToWorld().GetLocation();
}



void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->SetSound(FireSound);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SpawnProjectile()
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = this;
	ActorSpawnParams.Instigator = Cast<APawn>(GetOwner());
	FTransform Transform = Muzzle->GetComponentTransform();
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Transform.GetLocation(), Transform.GetRotation().Rotator(), ActorSpawnParams);

	if (projectile != nullptr)
	{
		FVector direction = FRotationMatrix(Transform.Rotator()).GetScaledAxis(EAxis::X);
		projectile->SetFireDirection(direction);

	}

	CurrentAmmo--;
}


