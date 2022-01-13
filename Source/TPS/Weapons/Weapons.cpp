// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons.h"
#include "CommonDefines.h"

// Sets default values
AWeapons::AWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	ProjectileClass = AProjectile::StaticClass();

	FireRate = 0.05f;

}

void AWeapons::LoadSkeletalMeshType(E_WEAPON_TYPE e_WeaponType)
{
	SetWeaponType(e_WeaponType);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_RIFLE(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PISTOL(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));

	switch (e_WeaponType)
	{
	case E_RIFLE :
		if (SK_RIFLE.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_RIFLE.Object);
			LOG_WARNING(TEXT("SetSkeletalMesh! RIfle"));
		}
		break;
	case E_PISTOL : 
		if (SK_PISTOL.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_PISTOL.Object);
			LOG_WARNING(TEXT("SetSkeletalMesh! Pistol"));
		}
		break;
	default:
		break;
	}

	// Default offset from the character location for projectiles to spawn
	SpawnOffset = FVector(0.0f, 35.0f, 8.0f);

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

void AWeapons::OnFire()
{
	//LOG_WARNING(TEXT("Weapon Fire!!!"));
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		//LOG_WARNING(TEXT("ProjectileClass!=NULL"));

		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		
		FVector MuzzleLocation = GetActorLocation() + FTransform(CameraRotation).TransformVector(SpawnOffset);
		FRotator MuzzleRotation = CameraRotation + FRotator(0.0f, 90.0f, 0.0f);
		
		//MuzzleRotation.Pitch += 10.0f; 
		UWorld* World = GetWorld(); 
			
		if (World) 
		{ 
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this; 
			SpawnParams.Instigator = GetInstigator();
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams); 
			if (Projectile) { 
				FVector LaunchDirection = MuzzleRotation.Vector();
				
				Projectile->FireInDirection(LaunchDirection); 
			} 
		}
	}
	else
	{
		LOG_WARNING(TEXT("ProjectileClass=NULL"));
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Weapon->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

// Called when the game starts or when spawned
void AWeapons::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
