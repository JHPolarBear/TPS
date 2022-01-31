// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Check_Target_InRange.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "Monsters/TPSAIController_MonsterBase.h"
#include "Characters/TPSCharacter.h"

UBTService_Check_Target_InRange::UBTService_Check_Target_InRange()
{
	NodeName = TEXT("CheckTargetInRange");
}

void UBTService_Check_Target_InRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	float FireRangeRadius = 600.f;
	bool IsInRange = false;

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;

	DrawDebugSphere(GetWorld(), ControllingPawn->GetActorLocation(), FireRangeRadius, 16, FColor::Red, false, 0.4f);

	ATPSCharacter* Player = Cast<ATPSCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATPSAIController_MonsterBase::TargetKey));
	if(Player)
	{
		

		if (ControllingPawn->GetDistanceTo(Player) <= FireRangeRadius)
		{
			IsInRange = true;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ATPSAIController_MonsterBase::IsAttackableKey, IsInRange);
}