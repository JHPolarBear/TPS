// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponLineTrace.h"
#include "Characters/TPSCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


void UWeaponLineTrace::OnFire( ATPSCharacter* Character )
{
	FVector WorldLocation;
	FVector WorldDirection;
	
	int x, y;

	Character->GetWorld()->GetFirstPlayerController()->GetViewportSize(x,y);
	Character->GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(x*0.5f,y*0.5f,WorldLocation,WorldDirection);

	FQuat rotator = FQuat(Character->GetControlRotation());
	FVector MuzzleLocation = Character->GetWeapon()->GetMuzzleLocation(); // + rotator.RotateVector(Character->Weapon->SpawnOffset);

	FVector start = MuzzleLocation;
	FVector end = WorldLocation;;//MuzzleLocation + rotator.GetForwardVector() * 10000.f;

	DrawDebugLine(Character->GetWorld(), start, end, FColor::Green, false, 0.5f);

	FCollisionQueryParams collision_params;
	collision_params.AddIgnoredActor(Character);

	FHitResult hit;

	Character->GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_PhysicsBody, collision_params);

	if (hit.GetActor() != NULL)
	{
		LOG_WARNING(TEXT("Hit! not null"));
		if (hit.GetActor()->IsRootComponentMovable())
		{
			USceneComponent* root = hit.GetActor()->GetRootComponent();
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(root);

			//RetNull(mesh);
			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
					mesh->AddForce(rotator.GetForwardVector() * 1e+5f * mesh->GetMass());

				LOG_WARNING(TEXT("Mesh Hit!"));
			}
		}
	}
}