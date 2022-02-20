// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"

#include "Characters/TPSPlayerState.h"
#include "Characters/TPSCharacter.h"
#include "GameSystem/TPSGameState.h"
#include "Weapons/Weapons.h"
#include "TPSUserWidget.h"

class ATPSCharacter;

ATPSPlayerController::ATPSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UTPSUserWidget> UI_TPS_C(TEXT("WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (UI_TPS_C.Succeeded())
	{
		TPSWidgetClass = UI_TPS_C.Class;
	}
}

void ATPSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATPSPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);


}

void ATPSPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (TeamID != NewTeamID)
	{
		TeamID = NewTeamID;
		// @todo notify perception system that a controller changed team ID
	}
}

void ATPSPlayerController::SettingWidget()
{
	TPSWidget = CreateWidget<UTPSUserWidget>(this, TPSWidgetClass);
	TPSWidget->AddToViewport(1);

	ATPSPlayerState* TPSPlayerState = Cast<ATPSPlayerState>(PlayerState);

	if (TPSPlayerState)
	{
		TPSPlayerState->OnPlayerStateChanged.Broadcast();

		ATPSCharacter* TPSCharacter = Cast<ATPSCharacter>(GetCharacter());

		if (TPSCharacter && !TPSCharacter->IsMonster)
		{
			TPSWidget->BindPlayerState(TPSPlayerState);

			AWeapons* pWeapon = TPSCharacter->GetWeapon();

			if (TPSWidget->BindWeapon(pWeapon))
			{
				pWeapon->OnWeaponStateChanged.Broadcast();
			}
		}
	}
	else
	{
		LOG_ERROR(TEXT("Failed to bind playerstate"));
	}

	ATPSGameState* TPSGameState = Cast<ATPSGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if(TPSGameState)
	{
		TPSWidget->BindGameState(TPSGameState);
	}
}