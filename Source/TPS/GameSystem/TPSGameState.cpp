// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/TPSGameState.h"

ATPSGameState::ATPSGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATPSGameState::Tick(float DeltaSeconds)
{
	ElapsedTime += GetWorld()->GetDeltaSeconds();

	OnTimeUpdate.Broadcast();
}

float ATPSGameState::GetRemainTime() const
{
	return MaxGameTime - ElapsedTime;
}
