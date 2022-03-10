// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonDefines.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "TPSAIController_MonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSAIController_MonsterBase : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController_MonsterBase();

	static const FName StartPosKey;
	static const FName NextPosKey;
	static const FName TargetKey;
	static const FName IsAttackableKey;
	static const FName HasLineOfSightKey;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void RunAI();
	void StopAI();

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& Actors);

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionInfoUpdated(const struct FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION(BlueprintNativeEvent)
	void LostSight();
	void LostSight_Implementation();

	UFUNCTION()
	void UpdateSightKey(bool _val);

	UFUNCTION()
	void UpdateTargetKey(AActor* actor);

private:

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	class UAISenseConfig_Sight* AISenseConfig_Sight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	float AIConfigSight_Radius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	float AIConfigSight_LoseSightRadius = 1400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	float AIConfigSight_NearClippingRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	float AIConfigSight_FieldOfView = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	FTimerHandle LostSightTimer;

	
};
