// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Check_Target_InRange.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UBTService_Check_Target_InRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Check_Target_InRange();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
