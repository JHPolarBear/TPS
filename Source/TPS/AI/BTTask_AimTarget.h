// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AimTarget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UBTTask_AimTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AimTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
