// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/TPSMonsterBase_StatComponent.h"

// Sets default values for this component's properties
UTPSMonsterBase_StatComponent::UTPSMonsterBase_StatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Init();

	// ...
}


// Called when the game starts
void UTPSMonsterBase_StatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTPSMonsterBase_StatComponent::SetMaxHP(float _val)
{
	MaxHP = _val;
}

float UTPSMonsterBase_StatComponent::GetMaxHP() const
{
	return MaxHP;
}

void UTPSMonsterBase_StatComponent::SetCurrentHP(float _newHP)
{
	CurrentHP = _newHP;

	// Broadcast
	OnStatChanged.Broadcast();

	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UTPSMonsterBase_StatComponent::GetCurrentHP() const
{
	return CurrentHP;
}

float UTPSMonsterBase_StatComponent::GetHPRatio()
{
	if (CurrentHP <= 0)
		return 0;
	else
		return CurrentHP / MaxHP;
}

void UTPSMonsterBase_StatComponent::SetDamage(float _damage)
{
	SetCurrentHP(FMath::Clamp(CurrentHP - _damage, 0.f, MaxHP));
}

void UTPSMonsterBase_StatComponent::SetMaxAP(float _val)
{
	MaxAP = _val;
}

float UTPSMonsterBase_StatComponent::GetMaxAP() const
{
	return MaxAP;
}

float UTPSMonsterBase_StatComponent::GetCurrentAP() const
{
	return CurrentAP;
}

float UTPSMonsterBase_StatComponent::GetAPRatio()
{
	if (CurrentAP <= 0.f)
		return 0.f;
	else
		return CurrentAP / MaxAP;
}

void UTPSMonsterBase_StatComponent::DecreaseAP()
{
	CurrentAP -= DecreaseAmountAP;
	CurrentAP = FMath::Clamp(CurrentAP, 0.f, MaxAP);
	OnStatChanged.Broadcast();
}

void UTPSMonsterBase_StatComponent::IncreaseAP()
{
	CurrentAP += DecreaseAmountAP;
	CurrentAP = FMath::Clamp(CurrentAP, 0.f, MaxAP);
	OnStatChanged.Broadcast();
}

void UTPSMonsterBase_StatComponent::SetRunMultiplier(float _val)
{
	RunMultiplier = _val;
}

float UTPSMonsterBase_StatComponent::GetRunMultiplier()
{
	return RunMultiplier;
}

void UTPSMonsterBase_StatComponent::SetDefaultWalkSpeed(float _val)
{
	DefaultWalkSpeed = _val;
}

float UTPSMonsterBase_StatComponent::GetDefaultWalkSpeed()
{
	return DefaultWalkSpeed;
}

void UTPSMonsterBase_StatComponent::Init()
{
	MaxHP = 100;
	MaxAP = 100;

	CurrentHP = 100;
	CurrentAP = 100;

	DecreaseAmountAP = 0.5;
}

