// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_StopAttack.h"

#include "Characters/TPSCharacter.h"
#include "Monsters/TPSAIController_MonsterBase.h"
#include "Monsters/TPSMonsterBase.h"

UBTTask_StopAttack::UBTTask_StopAttack()
{
	NodeName = TEXT("Stop Attack");
}

EBTNodeResult::Type UBTTask_StopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATPSMonsterBase* ControllingCharacter = Cast<ATPSMonsterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	if (ControllingCharacter == nullptr)
		return EBTNodeResult::Failed;

	ControllingCharacter->OnFireStop();


	return EBTNodeResult::Succeeded;
}
