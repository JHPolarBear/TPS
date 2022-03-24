// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/TPSCharacter.h"
#include "TPSPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSPlayer : public ATPSCharacter
{
	GENERATED_BODY()

public:
	ATPSPlayer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
