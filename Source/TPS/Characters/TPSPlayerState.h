// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
/**
 * 
 */
UCLASS()
class TPS_API ATPSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATPSPlayerState();

	void SetMaxHP(float _val);
	float GetMaxHP() const;
	float GetCurrentHP() const;
	float GetHPRatio();

	void SetMaxAP(float _val);
	float GetMaxAP() const;
	float GetCurrentAP() const;
	float GetAPRatio();

	int GetMaxBulletCount();
	int GetBulletCount();

	void SetMaxBulletCount(int _MaxBullet);
	void SetBulletCount(int _Bullet);

	void DecreaseAP();
	void IncreaseAP();

	void SetRunMultiplier(float _val);
	float GetRunMultiplier();

	void SetDefaultWalkSpeed(float _val);
	float GetDefaultWalkSpeed();

	void Init();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

private:
	/** HP: Character's life point  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		float CurrentHP;

	/** Action Point: Points indicating how long the character can dash */
	/** Action Point: 캐릭터가 대시할 수 있는 시간을 나타내는 포인트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		float MaxAP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		float CurrentAP;

	/** Amount of AP decrease for a frame when player runs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		float DecreaseAmountAP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		float DefaultWalkSpeed;

	/** Dash parameter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		float RunMultiplier;

	/** BulletCount parameter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		int BulletCount;

	/** MaxBulletCount parameter */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status, meta = (AllowPrivateAccess = true))
		int MaxBulletCount;
};
