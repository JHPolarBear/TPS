// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSMonsterBase_StatWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSMonsterBase_StatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void BindStatComponent(class UTPSMonsterBase_StatComponent* pStatComponent);

	void UpdateStatComponentData();	

private:
	TWeakObjectPtr<class UTPSMonsterBase_StatComponent> StatComponent;

	/** Monster's hp progress bar **/
	UPROPERTY();
	class UProgressBar* HPBar;
	
};
