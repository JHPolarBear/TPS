// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TPSGameState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(OnTimeUpdateDelegate)

UCLASS()
class TPS_API ATPSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATPSGameState();

	OnTimeUpdateDelegate OnTimeUpdate;

public:

	virtual void Tick( float DeltaSeconds ) override;

	float GetRemainTime() const;

private:
	UPROPERTY()
	float MaxGameTime = 60.f;

	UPROPERTY(Transient)
	float ElapsedTime  = 0.f;

	
};
