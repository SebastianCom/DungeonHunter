// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class MEGAMANCLONE_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckPoint();
	
	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UBoxComponent* BoxCollision;

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UPaperSpriteComponent* Sprite;

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UPaperSpriteComponent* TriggeredSprite;

	void Trigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
