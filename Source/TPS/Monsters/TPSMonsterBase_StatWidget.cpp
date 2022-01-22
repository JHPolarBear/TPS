// Fill out your copyright notice in the Description page of Project Settings.


#include "Monsters/TPSMonsterBase_StatWidget.h"

#include "Components/ProgressBar.h"
#include "Monsters/TPSMonsterBase_StatComponent.h"

void UTPSMonsterBase_StatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MONSTERBASE_PB_HPBAR")));
}

void UTPSMonsterBase_StatWidget::BindStatComponent(class UTPSMonsterBase_StatComponent* pStatComponent)
{
	if(pStatComponent == nullptr)
		return;

	StatComponent = pStatComponent;

	StatComponent->OnStatChanged.AddUObject(this, &UTPSMonsterBase_StatWidget::UpdateStatComponentData);
}

void UTPSMonsterBase_StatWidget::UpdateStatComponentData()
{
	if(StatComponent == nullptr)
		return;	

	HPBar->SetPercent(StatComponent->GetHPRatio());
}

