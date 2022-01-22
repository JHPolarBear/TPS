// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonDefines.h"
#include "Characters/TPSCharacter.h"
#include "TPSMonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSMonsterBase : public ATPSCharacter
{
	GENERATED_BODY()

public:
	ATPSMonsterBase();

	virtual void BeginPlay() override;

	float GetRandomPatrolRadiusMax()	{return RandomPatrolRadiusMax;}

public:
	void AimTarget(FVector TargetLocation);

	virtual void OnFire();

	virtual void OnFireStop();

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true) )
	float RandomPatrolRadiusMax = 1000.f;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UTPSMonsterBase_StatComponent* MonsterStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UWidgetComponent* StatWidget;
	
};
