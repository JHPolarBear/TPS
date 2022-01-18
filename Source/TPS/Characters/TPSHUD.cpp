// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TPSHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "CommonDefines.h"

ATPSHUD::ATPSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/unnamed.unnamed'"));
	if (CrosshairTexObj.Succeeded())
	{
		CrosshairTex = CrosshairTexObj.Object;
		LOG_WARNING(TEXT("HUD Create Success"));
	}
	else
		LOG_WARNING(TEXT("HUD Create Fail"));

	static ConstructorHelpers::FObjectFinder<UFont> HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	if (HUDFontOb.Succeeded())
	{
		HUDFont = HUDFontOb.Object;
	}
}


void ATPSHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X),(Center.Y + 20.0f));

	// 조준점 텍스쳐 필요
	//// draw the crosshair
	//FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	//TileItem.BlendMode = SE_BLEND_Translucent;
	//Canvas->DrawItem(TileItem);


	FString _String = FString::Printf(TEXT("."));
	DrawText(_String, FColor::Red, CrosshairDrawPosition.X, CrosshairDrawPosition.Y, HUDFont,1.5f);

}