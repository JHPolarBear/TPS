// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect_Player.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UBTService_Detect_Player : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Detect_Player();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
