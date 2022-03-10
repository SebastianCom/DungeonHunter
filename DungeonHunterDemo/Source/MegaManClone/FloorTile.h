// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

UCLASS()
class MEGAMANCLONE_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UBoxComponent* BoxCollision;

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UPaperSpriteComponent* Sprite;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
