// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonDefines.h"
#include "GameFramework/PlayerController.h"


#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ATPSPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	UPROPERTY()
	class UTPSUserWidget* TPSWidget;

	void SettingWidget();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UTPSUserWidget> TPSWidgetClass;

//----------------------------------------------------------------------//
// IGenericTeamAgentInterface
//----------------------------------------------------------------------//
// Bot ai와 팀 구분에 동일한 인터페이스를 사용하기 위해 추가
private:
	FGenericTeamId TeamID;

public:
	void SetGenericTeamId(const FGenericTeamId& NewTeamID);
	FGenericTeamId GetGenericTeamId() const { return TeamID; }

};
