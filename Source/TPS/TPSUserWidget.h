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

	void UpdateWeaponState();

	virtual void NativeConstruct() override;

protected:

	void UpdatePlayerState();

private:

	/** 현재 플레이어의 플레이어 스테이트 - 플레이어 스테이트에 선언된 캐릭터의 능력치를 UI에 바인딩하는데 사용   */
	TWeakObjectPtr<class ATPSPlayerState> CurrentPlayerState;

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
	/** 플레이어 AP : AP가 남아있을때만 플레이어 대쉬(달리기)가 가능 */
	UPROPERTY()
	class UProgressBar* APBar;

	TWeakObjectPtr<class AWeapons> CurrentWeapon;

	class UImage* ThumbnailImage = nullptr;
};