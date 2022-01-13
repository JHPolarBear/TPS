// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/TPSAIController_MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

const FName ATPSAIController_MonsterBase::StartPosKey(TEXT("StartPos"));
const FName ATPSAIController_MonsterBase::NextPosKey(TEXT("NextPos"));
const FName ATPSAIController_MonsterBase::TargetKey(TEXT("Target"));

ATPSAIController_MonsterBase::ATPSAIController_MonsterBase()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/Monsters/BB_TPSMonsterBase.BB_TPSMonsterBase'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/Monsters/BT_TPSMonsterBase.BT_TPSMonsterBase'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ATPSAIController_MonsterBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ATPSAIController_MonsterBase::OnUnPossess()
{
	Super::OnUnPossess();
}

void ATPSAIController_MonsterBase::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(StartPosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			LOG_ERROR(TEXT("Failed to run behavior tree"));
		}
	}
}

void ATPSAIController_MonsterBase::StopAI()
{
	auto BehaivorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if(BehaivorTreeComponent != nullptr)
	{
		BehaivorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
