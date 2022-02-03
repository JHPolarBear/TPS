// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UTPSUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindPlayerState(class ATPSPlayerState* NewPlayerState);

	bool BindWeapon(class AWeapons* weapon);

	void BindGameState(class ATPSGameState* NewGameState);

	void UpdateWeaponState();

	virtual void NativeConstruct() override;

protected:

	void UpdatePlayerState();

	void UpdateTimeState();

private:

	/** ���� �÷��̾��� �÷��̾� ������Ʈ - �÷��̾� ������Ʈ�� ����� ĳ������ �ɷ�ġ�� UI�� ���ε��ϴµ� ���   */
	TWeakObjectPtr<class ATPSPlayerState> CurrentPlayerState;

	/** ���� ���� ����� ���ӽ�����Ʈ */
	TWeakObjectPtr<class ATPSGameState> CurrentGameState;

	/** Player ID(NickName) */
	UPROPERTY()
	class UTextBlock* Projectile;

	/** Player ID(NickName) */
	UPROPERTY()
	class UTextBlock* PlayerName;

	/** Player remain HP */
	UPROPERTY()
	class UProgressBar* HPBar;

	/** Player remain AP(Dash Point): Character can dash while AP remains */
	/** �÷��̾� AP : AP�� ������������ �÷��̾� �뽬(�޸���)�� ���� */
	UPROPERTY()
	class UProgressBar* APBar;

	/** Elapsed time info **/
	UPROPERTY()
	class UTextBlock* ElapsedTimeText;

	TWeakObjectPtr<class AWeapons> CurrentWeapon;

	class UImage* ThumbnailImage = nullptr;
};