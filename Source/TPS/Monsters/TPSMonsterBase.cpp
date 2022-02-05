// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/TPSMonsterBase.h"

#include "Components/WidgetComponent.h"

#include "Monsters/TPSAIController_MonsterBase.h"
#include "Characters/TPSCharacter.h"
#include "Monsters/TPSMonsterBase_StatComponent.h"
#include "Monsters/TPSMonsterBase_StatWidget.h"

#include "Kismet/KismetMathLibrary.h"

ATPSMonsterBase::ATPSMonsterBase()
{
	// Disable Camera component's tick
	GetCameraBoom()->Deactivate();
	GetFollowCamera()->Deactivate();

	MonsterStat = CreateDefaultSubobject<UTPSMonsterBase_StatComponent>(TEXT("StatComponent"));

	StatWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatWidget"));
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_STATWIDGET(TEXT("WidgetBlueprint'/Game/UI/Characters/BP_MonsterBase_StatWidget.BP_MonsterBase_StatWidget_C'"));
	if(UI_STATWIDGET.Succeeded())
	{
		StatWidget->SetWidgetClass(UI_STATWIDGET.Class);
		StatWidget->SetDrawSize(FVector2D(110.f, 10.f));
	}

	StatWidget->SetupAttachment(GetMesh());
	StatWidget->SetRelativeLocation(FVector(0.f, 0.f, 205.f));
	StatWidget->SetWidgetSpace(EWidgetSpace::Screen);


	// Set AI Controller as default
	AIControllerClass = ATPSAIController_MonsterBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed *= 0.6f;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 240.f, 0.f);
	
	IsMonster = true;

	DeadActionTime = 2.0f;
}

void ATPSMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	// Start ai behavior tree
	ATPSAIController_MonsterBase* TPSAIController = Cast<ATPSAIController_MonsterBase>(GetController());
	if(TPSAIController)
	{
		TPSAIController->RunAI();
	}

	// bind statcomponent to stat widget
	UTPSMonsterBase_StatWidget* MonsterStatWidget = Cast<UTPSMonsterBase_StatWidget>(StatWidget->GetUserWidgetObject());
	if(MonsterStatWidget)
		MonsterStatWidget->BindStatComponent(MonsterStat);

	if(MonsterStat)
	{
		MonsterStat->OnHPIsZero.AddLambda([this]()->void{
			OnDead();
		});
	}

	GetCharacterMovement()->MaxWalkSpeed /= 0.7;
}

float ATPSMonsterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage 받기.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (0 == (PointDamageEvent->HitInfo.BoneName).Compare(FName(TEXT("Head"))))
		{
			FinalDamage *= 10; // 맞은 부위가 Head면, 데미지 3배.
			LOG_WARNING(TEXT("HEAD! HEAD! HEAD! HEAD! HEAD!"));
		}
	}
	// RadialDamage 받기.
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
	}

	MonsterStat->SetDamage(FinalDamage);

	//LOG_WARNING(TEXT("Take Damage!! Monster"));

	return FinalDamage;
}

void ATPSMonsterBase::AimTarget(FVector TargetLocation)
{
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);

	SetActorRelativeRotation(FQuat(LookAt));
}

void ATPSMonsterBase::OnFire()
{
	if (Weapon->nCurrentBulletNum <= 0 && Weapon->bIsReloading == false)
	{
		OnReloadingStart();
		return;
	}
	else if(Weapon->bIsReloading)
	{
		return;
	}

	if(GetIsFiring() == false)
	{
		LOG_WARNING(TEXT("MonsterBase On Fire!!!"));

		SetIsFiring(true);

		// 첫발을 위해서 초기 셋팅
		FireDeltaTime = Weapon->GetFireRate();
	}
}

void ATPSMonsterBase::OnFireStop()
{
	//LOG_WARNING(TEXT("MonsterBase On Fire Stop!!!"));

	Super::OnFireStop();
}

void ATPSMonsterBase::OnDead()
{
	if(GetController())
	{
		StartDeathAction();

		// 향후에는 모든 액션을 멈추는 기능으로 통합할 것
		OnFireStop();

		// Start ai behavior tree
		ATPSAIController_MonsterBase* TPSAIController = Cast<ATPSAIController_MonsterBase>(GetController());
		if (TPSAIController)
		{
			TPSAIController->StopAI();
		}

		GetController()->StopMovement();
		GetController()->UnPossess();

		// 더 이상 충돌을 일으키지 않도록 (시체 위를 지나갈수 있도록 등)
		SetActorEnableCollision(false);

		SetCanBeDamaged(false);

		// 관련 위젯 비활성화
		StatWidget->SetHiddenInGame(true);

		GetWorld()->GetTimerManager().SetTimer(DeadActionHandler, this, &ATPSMonsterBase::OnDeadAction, DeadActionTime, false);
	}
}

void ATPSMonsterBase::OnDeadAction()
{
	if(Weapon)
		Weapon->Destroy();

	Destroy();
}
