// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNextPos_Random.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UBTTask_FindNextPos_Random : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindNextPos_Random();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
