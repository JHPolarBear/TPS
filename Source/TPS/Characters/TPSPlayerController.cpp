// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"

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