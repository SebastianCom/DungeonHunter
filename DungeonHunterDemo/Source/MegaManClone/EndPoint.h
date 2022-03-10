// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndPoint.generated.h"

UCLASS()
class MEGAMANCLONE_API AEndPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndPoint();

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UBoxComponent* BoxCollision;

	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UPaperSpriteComponent* Sprite;


	UPROPERTY(Category = Character, VisibleAnywhere, meta = (AllowPrivateAccess))
		class UPaperSpriteComponent* TriggeredSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* EndSound;

	void Trigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
