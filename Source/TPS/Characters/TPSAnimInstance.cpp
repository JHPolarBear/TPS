// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TPSAnimInstance.h"

UTPSAnimInstance::UTPSAnimInstance()
{
	Velocity = 0.0f;

	IsAttack = false;

	State = (int) E_CONTROL_MODE::NORMAL;
}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		Velocity = Pawn->GetVelocity().Size();
	}
}