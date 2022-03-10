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

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaSeconds) override;

	float GetRandomPatrolRadiusMax()	{return RandomPatrolRadiusMax;}

public:
	void AimTarget(FVector TargetLocation);

	virtual void OnFire();

	virtual void OnFireStop();

	void OnDead();

	void OnDeadAction();

	void SetSkeletonTeamColor();

	// 사망 시 동작하는 타이머
	FTimerHandle DeadActionHandler;
	// 사망 후 잔여 시간
	float DeadActionTime; 

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true) )
	float RandomPatrolRadiusMax = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = true))
	int	TeamID = 255;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UTPSMonsterBase_StatComponent* MonsterStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UWidgetComponent* StatWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = true))
	class UMaterialInstanceDynamic* DynamicBodyMaterial;
	
};
