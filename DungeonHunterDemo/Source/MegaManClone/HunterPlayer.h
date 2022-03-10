// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "HunterPlayer.generated.h"

/**
 * 
 */
enum class EInventoryDirection : unsigned char;

UCLASS()
class MEGAMANCLONE_API AHunterPlayer : public APaperCharacter
{
	GENERATED_BODY()

public:
	
	AHunterPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* MeeleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* ShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* ShootAnimation_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class UBoxComponent* MeeleBox;

	class UPaperFlipbook* LastAttackAnimation;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	void MoveRight(float value);
	void UpdateAnimation();
	void UpdateCharacter();

	virtual void Jump() override;

	void Attack();
	void EndAttack();
	void ResetWeaponDelay();
	void Land();
	void Die();
	void Repawn();


	float WeaponDelay;
	bool bIsAlive;
	bool bCanBeDamaged;
	bool bCanAttack;
	bool bOnFloor;
	bool DmgDealt;
	int MeeleDmg;

	FTimerHandle timerHandle;
	FTimerHandle attackDelayHandle;
	FTimerHandle ShootDelayHandle;
	FTimerHandle DeathHandle;
	FTimerHandle EndHandle;

	void ActivateCollision(UBoxComponent* Comp);
	void DeactivateCollision(UBoxComponent* Comp);

	UFUNCTION()
		void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class USceneComponent* CharacterScene;

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class USphereComponent* PickupSphereComponent;

	UFUNCTION()
		void NextWeapon();

	UFUNCTION()
		void PrevWeapon();

	UFUNCTION()
		void PickupWeapon();
	
	UFUNCTION()
		void Shoot();

	UFUNCTION()
		void StopShoot();
	
	UFUNCTION()
		void PlayShootAnimation();

	UFUNCTION()
		void EndGame();

	class AWeapon* CurrentWeapon;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
		bool HasWeapon();

	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetCurrentAmmo();
	
	UFUNCTION(BlueprintCallable, Category = "Ammo")
		int GetMaxAmmo();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetMaxHealth();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* PistolEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* OutOfAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* LandSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* WalkSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* MeeleSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* HurtSound;
	
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnNoiseEmitterComponent* NoiseEmitterComponent;
	UFUNCTION()
		void MakeSomeNoise();


protected:
	virtual void BeginPlay() override;

	float FireRateTimer = 10.0f;

	void EquipFromInventory(EInventoryDirection dir);
	
	UFUNCTION()
		void OnPickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
