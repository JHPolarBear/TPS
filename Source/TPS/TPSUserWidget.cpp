// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSUserWidget.h"
#include "Components/Image.h" 
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Characters/TPSPlayerState.h"
#include "Characters/TPSCharacter.h"
#include "Weapons/Weapons.h"

// 무기 정보
bool UTPSUserWidget::BindWeapon(class AWeapons* pWeapon)
{
	if (pWeapon != nullptr)
	{
		CurrentWeapon = pWeapon;
		CurrentWeapon->OnWeaponStateChanged.AddUObject(this, &UTPSUserWidget::UpdateWeaponState);

		LOG_WARNING(TEXT("Success to bind weapon"));

		return true;
	}
	else
	{
		LOG_ERROR(TEXT("Failed to bind weapon - weapon is nullptr"));
		return false;
	}

}

void UTPSUserWidget::UpdateWeaponState()
{
	auto CurrentCharacter= UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (CurrentCharacter)
	{
		ATPSCharacter* pCharacter = Cast<ATPSCharacter>(CurrentCharacter);

		if (pCharacter)
		{
			FString ThumbnailName = pCharacter->GetWeapon()->GetThumbnailPath();

			UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ThumbnailName));

			if (Texture)
			{
				ThumbnailImage->SetBrushFromSoftTexture(Texture);
				LOG_WARNING(TEXT("Success to create texture"));
			}
			else
			{
				//LOG_WARNING(TEXT("Failed to create texture"));
			}
		}
	}
}

void UTPSUserWidget::BindPlayerState(class ATPSPlayerState* NewPlayerState)
{
	CurrentPlayerState = NewPlayerState;
	NewPlayerState->OnPlayerStateChanged.AddUObject(this, &UTPSUserWidget::UpdatePlayerState);
}

void UTPSUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName_Slot")));
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HP_Bar")));
	APBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("AP_Bar")));
	ThumbnailImage = Cast<UImage>(GetWidgetFromName(TEXT("WeaponThumbnail_IMG")));
}

void UTPSUserWidget::UpdatePlayerState()
{
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));

	HPBar->SetPercent(CurrentPlayerState->GetHPRatio());
	APBar->SetPercent(CurrentPlayerState->GetAPRatio());
}
