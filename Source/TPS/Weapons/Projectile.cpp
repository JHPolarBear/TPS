// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "CommonDefines.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "Monsters/TPSMonsterBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetHiddenInGame(true);
	CollisionComponent->SetEnableGravity(false);
	CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	// Set as root component
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_545x39.SM_Shell_545x39'"));
	if (MESH.Succeeded())
	{
		MeshComponent->SetStaticMesh(MESH.Object);
		MeshComponent->SetRelativeRotation( FRotator(0, -90.0f, 0) );
		MeshComponent->SetWorldScale3D(FVector(5.0f));
		MeshComponent->SetupAttachment(CollisionComponent);
		MeshComponent->SetEnableGravity(false);
	}

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	//  중력 없이
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// Die after 10 seconds by default
	InitialLifeSpan = 1.0f;
	//LOG_WARNING(TEXT("PROJECTILE CREATE!!"));

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->bAutoActivate = false;
	ParticleSystemComponent->SetHiddenInGame(false);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_Trap_Charge_Fire_01.P_Trap_Charge_Fire_01'"));

	if (ParticleAsset.Succeeded())
	{
		ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
	}
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComponent != NULL)/* && OtherComponent->IsSimulatingPhysics()*/)
	{
		if(ParticleSystemComponent && ParticleSystemComponent->Template)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemComponent->Template, GetActorLocation());
		}
	}

	auto Character = Cast<ATPSCharacter>(OtherActor);
	if (Character && !Character->IsMonster)
	{
		FDamageEvent DamageEvent;
		Character->TakeDamage(1, DamageEvent, nullptr, this);
		LOG_WARNING(TEXT("Hit!! Player"));
	}

	auto Monster = Cast<ATPSMonsterBase>(OtherActor);
	if(Monster)
	{
		FDamageEvent DamageEvent;
		Monster->TakeDamage(10, DamageEvent, nullptr, this);
		LOG_WARNING(TEXT("Hit!! Monster"));
	}

	Destroy();
}