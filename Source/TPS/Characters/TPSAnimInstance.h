// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterDefines.h"
#include "TPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		float Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		float AimVertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		float AimHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		int State;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		bool IsFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, meta = (AllowPrivateAccess = "true"))
		bool IsReloading;

public:
	UTPSAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void SetDeathState();
	void SetState(int set_State) { State = set_State; };

	bool GetIsFireCheck() { return IsFire; };
	void SetIsFire(bool set_Fire) { IsFire = set_Fire; };
	void SetAimVertical(float vertical) { AimVertical = vertical; };
	void SetAimHorizontal(float horizontal) { AimHorizontal = horizontal; };
	void SetIsReloading(bool set_Reloading) { IsReloading = set_Reloading; };

	float GetAimVertical() { return AimVertical; };
	float GetAimHorizontal() { return AimHorizontal; };


};
