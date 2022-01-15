// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsInAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/TPSCharacter.h"
#include "Monsters/TPSAIController_MonsterBase.h"

#include "DrawDebugHelpers.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControllingPawn == nullptr)
		return false;

	auto Target = Cast<ATPSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATPSAIController_MonsterBase::TargetKey));
	if(Target == nullptr)
		return false;
	
	float FireRange = 300.f;

	FVector Center = ControllingPawn->GetActorLocation();

	DrawDebugSphere(ControllingPawn->GetWorld(), Center, FireRange, 16, FColor::Blue, false, 0.4f, 10.f);

	bResult = (Target->GetDistanceTo(ControllingPawn) <= FireRange);
	return bResult;
}
