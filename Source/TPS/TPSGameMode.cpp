// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSGameMode.h"
#include "Characters/TPSCharacter.h"
#include "Characters/TPSPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Characters/TPSHUD.h"

ATPSGameMode::ATPSGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));

	// use our custom HUD class
	HUDClass = ATPSHUD::StaticClass();

	DefaultPawnClass = ATPSCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();


	LOG_WARNING(TEXT("Game Mode Created"));
}

void ATPSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	LOG_WARNING(TEXT("Post Login"));
}