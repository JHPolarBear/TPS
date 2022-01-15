// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "Characters/TPSCharacter.h"
#include "Monsters/TPSAIController_MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Monsters/TPSMonsterBase.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATPSMonsterBase* ControllingCharacter = Cast<ATPSMonsterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	if(ControllingCharacter == nullptr)
		return EBTNodeResult::Failed;

	
	auto Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATPSAIController_MonsterBase::TargetKey);
	if(Target == nullptr)
		return EBTNodeResult::Failed;

	ATPSCharacter* TPSCharacterTarget = Cast<ATPSCharacter>(Target);
	if(TPSCharacterTarget == nullptr)
		return EBTNodeResult::Failed;
	FVector TargetPos = TPSCharacterTarget->GetActorLocation();

	ControllingCharacter->AimTarget(TargetPos);

	ControllingCharacter->OnFire();	

	return EBTNodeResult::Succeeded;
}
