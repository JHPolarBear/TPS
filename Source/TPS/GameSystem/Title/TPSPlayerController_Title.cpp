// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Title/TPSPlayerController_Title.h"

#include "Blueprint/UserWidget.h"


void ATPSPlayerController_Title::BeginPlay()
{
	Super::BeginPlay();

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	ASSERT_CHECK(UIWidgetInstance != nullptr);


	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());

	SetInputMode(Mode);
	bShowMouseCursor = true;
}
