// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TPSAnimInstance.h"

UTPSAnimInstance::UTPSAnimInstance()
{
	Velocity = 0.0f;
	Direction = 0.0f;

	IsFire = false;
	IsReloading = false;

	State = (int) E_CONTROL_MODE::NORMAL;
}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		Velocity = Pawn->GetVelocity().Size();
		Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
	}
}
void UTPSAnimInstance::SetDeathState() 
{
	State = (int)E_CONTROL_MODE::DEATH;
}