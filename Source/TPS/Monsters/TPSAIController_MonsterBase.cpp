// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/TPSAIController_MonsterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AIPerceptionComponent.h"


#include "BehaviorTree/BehaviorTree.h"

#include "Characters/TPSCharacter.h"

const FName ATPSAIController_MonsterBase::StartPosKey(TEXT("StartPos"));
const FName ATPSAIController_MonsterBase::NextPosKey(TEXT("NextPos"));
const FName ATPSAIController_MonsterBase::TargetKey(TEXT("Target"));
const FName ATPSAIController_MonsterBase::IsAttackableKey(TEXT("IsAttackable"));

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

	// AI Perception
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	ASSERT_CHECK(GetPerceptionComponent() != nullptr);

	AISenseConfig_Sight = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Sight"));
	ASSERT_CHECK(AISenseConfig_Sight);

	AISenseConfig_Sight->SightRadius = AIConfigSight_Radius;
	AISenseConfig_Sight->LoseSightRadius = AIConfigSight_LoseSightRadius;
	AISenseConfig_Sight->NearClippingRadius = AIConfigSight_NearClippingRadius;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = AIConfigSight_FieldOfView;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->ConfigureSense(*AISenseConfig_Sight);

	GetPerceptionComponent()->SetDominantSense(AISenseConfig_Sight->GetSenseImplementation());

	// set ai's team ID (For distinguish enemy and friend)
	SetGenericTeamId(FGenericTeamId(10));

	// AI Perception으로 캐릭터 감지 시 해야 할 일을 세팅
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ATPSAIController_MonsterBase::OnPerceptionUpdated);
	GetPerceptionComponent()->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ATPSAIController_MonsterBase::OnTargetPerceptionInfoUpdated);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ATPSAIController_MonsterBase::OnTargetPerceptionUpdated);
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

void ATPSAIController_MonsterBase::OnPerceptionUpdated(const TArray<AActor*>& Actors)
{
	// 감지된 모든 액터를 전달 받아 처리해야 할 경우
}

void ATPSAIController_MonsterBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ATPSCharacter* Player = Cast<ATPSCharacter>(Actor);

	if(Player)
	{
		LOG_WARNING(TEXT("Player Pos : [ %f, %f]"), Player->GetActorLocation().X, Player->GetActorLocation().Y);
	}
}

void ATPSAIController_MonsterBase::OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	ATPSCharacter* Player = Cast<ATPSCharacter>(UpdateInfo.Target);

	if(Player)
	{
		if(UpdateInfo.Stimulus.WasSuccessfullySensed() == false)
			Blackboard->SetValueAsObject(TargetKey, nullptr);
		else
		{
			ETeamAttitude::Type OtherAttr = ETeamAttitude::Neutral;

			ATPSCharacter* AIOwner = Cast<ATPSCharacter>(GetPawn());
			if(AIOwner)
			{
				OtherAttr =  AIOwner->CompareTeamAttribute(*Player);
			}

			if(OtherAttr == ETeamAttitude::Hostile)
				Blackboard->SetValueAsObject(TargetKey, Player);
		}
	}
}