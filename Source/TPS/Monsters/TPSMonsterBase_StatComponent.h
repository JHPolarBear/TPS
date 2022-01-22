// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSMonsterBase_StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(OnStatChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UTPSMonsterBase_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSMonsterBase_StatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void SetMaxHP(float _val);
	float GetMaxHP() const;
	float GetCurrentHP() const;
	float GetHPRatio();

	void SetMaxAP(float _val);
	float GetMaxAP() const;
	float GetCurrentAP() const;
	float GetAPRatio();

	void DecreaseAP();
	void IncreaseAP();

	void SetRunMultiplier(float _val);
	float GetRunMultiplier();

	void SetDefaultWalkSpeed(float _val);
	float GetDefaultWalkSpeed();

	void Init();

	OnStatChangedDelegate OnStatChanged;

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

		
};
