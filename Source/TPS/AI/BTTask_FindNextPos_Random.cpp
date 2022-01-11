// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindNextPos_Random.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monsters/TPSMonsterBase.h"
#include "Monsters/TPSAIController_MonsterBase.h"

UBTTask_FindNextPos_Random::UBTTask_FindNextPos_Random()
{
	NodeName = TEXT("FindNextPos_Random");
}

EBTNodeResult::Type UBTTask_FindNextPos_Random::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControlloingCharacter = Cast<ATPSMonsterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	if (ControlloingCharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(OwnerComp.GetAIOwner()->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector crntPos = ControlloingCharacter->GetActorLocation();
	float fPatrolRadius = ControlloingCharacter->GetRandomPatrolRadiusMax();

	FNavLocation nextPos;
	if (NavSystem->GetRandomPointInNavigableRadius(crntPos, fPatrolRadius, nextPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ATPSAIController_MonsterBase::StartPosKey, crntPos);

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ATPSAIController_MonsterBase::NextPosKey, nextPos);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
