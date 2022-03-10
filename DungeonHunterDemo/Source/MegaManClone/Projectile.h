// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class MEGAMANCLONE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void SetFireDirection(FVector dir);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovementComponent;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Set In Editor for Different projectile types
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int MaximumSpeed = 20000;

	//Set In Editor for Different projectile types
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int InitialSpeed = 5000;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SceneComponent;

	/* sphere collision component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComponent;

	/** paper flipbook component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent* ProjectileSprite;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
		int32 ShotDmg;

	/** calls when this actor overlaps another  */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
