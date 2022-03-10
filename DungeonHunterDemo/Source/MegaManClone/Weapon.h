// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class MEGAMANCLONE_API AWeapon : public APickUpActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int MaximumAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	AWeapon();

	void Equip(class AHunterPlayer* Character);
	void UnEquip();

	UFUNCTION()
		void Fire();



	FVector GetMuzzleLocation() const;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SpawnProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Muzzle;



};
