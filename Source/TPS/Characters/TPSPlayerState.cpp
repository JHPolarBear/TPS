// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TPSPlayerState.h"


ATPSPlayerState::ATPSPlayerState()
{
	Init();
}

void ATPSPlayerState::SetMaxHP(float _val)
{
	MaxHP = _val;
	OnPlayerStateChanged.Broadcast();
}

void ATPSPlayerState::SetCurrentHP(float _val)
{
	CurrentHP = _val;
	OnPlayerStateChanged.Broadcast();
}

float ATPSPlayerState::GetMaxHP() const
{
	return MaxHP;
}

float ATPSPlayerState::GetCurrentHP() const
{
	return CurrentHP;
}

float ATPSPlayerState::GetHPRatio()
{
	if (CurrentHP <= 0)
		return 0;
	else
		return CurrentHP / MaxHP;
}

void ATPSPlayerState::SetMaxAP(float _val)
{
	MaxAP = _val;
}

float ATPSPlayerState::GetMaxAP() const
{
	return MaxAP;
}

float ATPSPlayerState::GetCurrentAP() const
{
	return CurrentAP;
}

float ATPSPlayerState::GetAPRatio()
{
	if (CurrentAP <= 0.f)
		return 0.f;
	else
		return CurrentAP / MaxAP;
}

void ATPSPlayerState::DecreaseAP()
{
	CurrentAP -= DecreaseAmountAP;
	CurrentAP = FMath::Clamp(CurrentAP, 0.f, MaxAP);
	OnPlayerStateChanged.Broadcast();
}

void ATPSPlayerState::IncreaseAP()
{
	CurrentAP += DecreaseAmountAP;
	CurrentAP = FMath::Clamp(CurrentAP, 0.f, MaxAP);
	OnPlayerStateChanged.Broadcast();
}

void ATPSPlayerState::SetDamage(float _damage)
{
	SetCurrentHP(FMath::Clamp(CurrentHP - _damage, 0.f, MaxHP));
}

int ATPSPlayerState::GetMaxBulletCount()
{
	return MaxBulletCount;
}

int ATPSPlayerState::GetBulletCount()
{
	return BulletCount;
}
void ATPSPlayerState::SetMaxBulletCount(int _MaxBullet)
{
	MaxBulletCount = _MaxBullet;
	OnPlayerStateChanged.Broadcast();
}

void ATPSPlayerState::SetBulletCount(int _Bullet)
{
	BulletCount = _Bullet;
	OnPlayerStateChanged.Broadcast();
}

void ATPSPlayerState::SetRunMultiplier(float _val)
{
	RunMultiplier = _val;
}

float ATPSPlayerState::GetRunMultiplier()
{
	return RunMultiplier;
}

void ATPSPlayerState::SetDefaultWalkSpeed(float _val)
{
	DefaultWalkSpeed = _val;
}

float ATPSPlayerState::GetDefaultWalkSpeed()
{
	return DefaultWalkSpeed;
}

void ATPSPlayerState::Init()
{
	MaxHP = 100;
	MaxAP = 100;

	CurrentHP = 100;
	CurrentAP = 100;

	DecreaseAmountAP = 0.5f;
	
	MaxBulletCount = 0;
	BulletCount = 0;

}
