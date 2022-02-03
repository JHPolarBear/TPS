// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameMode.h"
#include "Characters/TPSCharacter.h"
#include "Characters/TPSPlayerController.h"
#include "GameSystem/TPSGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Characters/TPSHUD.h"

ATPSGameMode::ATPSGameMode()
	: Super()
{
	// 전용 플레이어 스테이트 사용
	DefaultPawnClass = ATPSCharacter::StaticClass();
	// use our custom HUD class
	HUDClass = ATPSHUD::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	PlayerStateClass = ATPSPlayerState::StaticClass();
	GameStateClass = ATPSGameState::StaticClass();

	LOG_WARNING(TEXT("Game Mode Created"));
}

void ATPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	LOG_WARNING(TEXT("Post Login"));
}

void ATPSGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
