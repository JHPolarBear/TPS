// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/TPSMonsterBase.h"
#include "Monsters/TPSAIController_MonsterBase.h"
#include "Characters/TPSCharacter.h"

#include "Kismet/KismetMathLibrary.h"

ATPSMonsterBase::ATPSMonsterBase()
{
	// Disable Camera component's tick
	GetCameraBoom()->Deactivate();
	GetFollowCamera()->Deactivate();

	// Set AI Controller as default
	AIControllerClass = ATPSAIController_MonsterBase::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed *= 0.6f;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	IsMonster = true;
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
}

void ATPSMonsterBase::AimTarget(FVector TargetLotation)
{
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLotation);

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
	LOG_WARNING(TEXT("MonsterBase On Fire Stop!!!"));

	Super::OnFireStop();
}
