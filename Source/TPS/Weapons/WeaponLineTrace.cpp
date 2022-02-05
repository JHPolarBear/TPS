// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponLineTrace.h"
#include "Characters/TPSCharacter.h"
#include "Monsters/TPSMonsterBase.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


void UWeaponLineTrace::OnFire( ATPSCharacter* Character )
{
	int x, y;
	FHitResult hit;

	Character->GetWorld()->GetFirstPlayerController()->GetViewportSize(x,y);
	Character->GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(FVector2D(x * 0.5f, y * 0.5f), ECC_Visibility, false, hit);
	
	FQuat rotator = FQuat(Character->GetControlRotation());
	FVector MuzzleLocation = Character->GetWeapon()->GetMuzzleLocation();

	FVector start = MuzzleLocation;
	FVector end = FVector::ZeroVector;

	if (hit.GetActor() != NULL)
	{
		end = FVector(hit.Location.X, hit.Location.Y, hit.Location.Z);
	}

	FCollisionQueryParams collision_params;
	collision_params.AddIgnoredActor(Character);

	FColor Line_Color = FColor::Green;

	Character->GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_PhysicsBody, collision_params);

	if (hit.GetActor() != NULL)
	{
		//LOG_WARNING(TEXT("Hit! not null"));

		if (hit.GetActor()->IsRootComponentMovable())
		{
			LOG_WARNING(TEXT("Movable Hit!"));
			Line_Color = FColor::Red;
		}
	}

	DrawDebugLine(Character->GetWorld(), start, end, Line_Color, false, 1.0f);

}