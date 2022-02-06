// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons.h"
#include "CommonDefines.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/TPSCharacter.h"
#include "WeaponLineTrace.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeapons::AWeapons()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	ProjectileClass = AProjectile::StaticClass();

	FireRate = 0.1f;

	bIsReloading = false;
	MuzzleLocation = FVector::ZeroVector;
	MuzzleRotation = FRotator::ZeroRotator;

	LineTrace = NewObject<UWeaponLineTrace>();	
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

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	// Default offset from the character location for projectiles to spawn
	SpawnOffset = FVector(0.0f, 35.0f, 8.0f);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->bAutoActivate = false;
	ParticleSystemComponent->SetHiddenInGame(false);
	ParticleSystemComponent->SetRelativeLocation(SpawnOffset);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Effects/P_Ranged_Firey_Impact.P_Ranged_Firey_Impact'"));

	if (ParticleAsset.Succeeded())
	{
		ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(Weapon);

	static ConstructorHelpers::FObjectFinder<USoundBase> FireSound(TEXT("SoundCue'/Game/Sound/FireSound.FireSound'"));
	if (FireSound.Succeeded())
	{
		AudioComponent->SetSound(FireSound.Object);
	}
}

void AWeapons::OnFire(ATPSCharacter* Character)
{
	//LOG_WARNING(TEXT("Weapon Fire!!!"));
	// try and fire a projectile

	if(Character->IsPlayerControlled() == false)
	{
		OnFire_AI(Character);
		return;
	}
	
	// 현재 총알이남았고, 재장전 아닐때~
	if ( nCurrentBulletNum > 0 && !bIsReloading )
	{
		if (ProjectileClass != NULL)
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);
			
			int x, y;
			//  컨트롤러 기준에서 중심점 월드 좌표 얻고
			Character->GetWorld()->GetFirstPlayerController()->GetViewportSize(x, y);

			MuzzleLocation = GetActorLocation() + FTransform(CameraRotation).TransformVector(SpawnOffset);
			MuzzleRotation = GetActorRotation();

			FVector FireDirection;// = WorldDirection;

			FHitResult hit;
			Character->GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(FVector2D(x * 0.5f, y * 0.5f), ECC_Visibility, false, hit);
			if (hit.GetActor() != NULL)
			{
				// 새로 찾은 월드 좌표에서 소켓 위치 기준으로 방향 다시 잡음
				FVector EndPoint = FVector(hit.Location.X, hit.Location.Y, hit.Location.Z);
				FireDirection = (EndPoint - MuzzleLocation).GetSafeNormal();
				//LOG_WARNING(TEXT("Target Check!"));
			}

			UWorld* World = Character->GetWorld(); 
		
			if (World) 
			{ 
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this; 
				SpawnParams.Instigator = GetInstigator();
				AudioComponent->Play();

				if (ParticleSystemComponent)
				{
					UGameplayStatics::SpawnEmitterAttached(ParticleSystemComponent->Template, Weapon, FName("Muzzle"));
				}

				AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams); 
				if (Projectile) 
				{
					Projectile->FireInDirection(FireDirection);
				} 
			}
		}
		else
		{
			LOG_WARNING(TEXT("ProjectileClass=NULL"));
		}

		bool bLinTraceTest = true;

		if (LineTrace && bLinTraceTest)
			LineTrace->OnFire(Character);

		nCurrentBulletNum--;
	}
}

void AWeapons::OnFire_AI(ATPSCharacter* Character)
{
	if(nCurrentBulletNum > 0 && bIsReloading == false)
	{
		FRotator CharacterRotation = Character->GetActorRotation();

		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		MuzzleRotation = CharacterRotation;
		MuzzleLocation = GetActorLocation() + CameraRotation.RotateVector(SpawnOffset);

		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			AudioComponent->Play();

			if (ParticleSystemComponent)
			{
				UGameplayStatics::SpawnEmitterAttached(ParticleSystemComponent->Template, Weapon, FName("Muzzle"));
			}
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		}

		nCurrentBulletNum--;
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
