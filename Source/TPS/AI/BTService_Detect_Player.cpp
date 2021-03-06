// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect_Player.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

#include "Monsters/TPSAIController_MonsterBase.h"
#include "Characters/TPSCharacter.h"

UBTService_Detect_Player::UBTService_Detect_Player()
{
	NodeName = TEXT("DetectPlayer");
}

void UBTService_Detect_Player::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ControllingPawn == nullptr)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();

	float DetectRadius = 1000.f;
	float FireRangeRadius = 600.f;

	ATPSCharacter* TPSCharacterTarget = nullptr;
	bool IsAttackable = false;

	if(World == nullptr)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, ControllingPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.4f);
	DrawDebugSphere(World, Center, FireRangeRadius, 16, FColor::Blue, false, 0.4f);

	if(bResult)
	{
		for(auto OverlapResult : OverlapResults)
		{
			ATPSCharacter* TPSCharacter = Cast<ATPSCharacter>(OverlapResult.GetActor());
			if(TPSCharacter && TPSCharacter->GetController()->IsPlayerController())
			{
				TPSCharacterTarget = TPSCharacter;

				DrawDebugPoint(World, TPSCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);

				DrawDebugLine(World, Center, TPSCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);

				if(ControllingPawn->GetDistanceTo(TPSCharacter) <= FireRangeRadius)
				{
					IsAttackable = true;
				}

				break;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATPSAIController_MonsterBase::TargetKey, TPSCharacterTarget);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ATPSAIController_MonsterBase::IsAttackableKey, IsAttackable);
}
