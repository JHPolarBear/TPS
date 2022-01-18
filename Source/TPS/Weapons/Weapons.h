// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDefines.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapons.generated.h"

UCLASS(config = Game)
class TPS_API AWeapons : public AActor
{
	GENERATED_BODY()


public:	

	// Sets default values for this actor's properties
	AWeapons();

	E_WEAPON_TYPE mWeponType;

	int nMaxBulletNum;
	int nCurrentBulletNum;
	bool bIsReloading;

	bool bFullAutoFire;
	float FireRate;
	float CurrentFireDeltaTime;
	
	FVector MuzzleLocation;
	FRotator MuzzleRotation;

	FVector GetMuzzleLocation() {return MuzzleLocation; };

	void SetWeaponType(E_WEAPON_TYPE e_WeaponType) { mWeponType = e_WeaponType;  };

	//E_WEAPON_TYPE GetCurrWeaponType() { return e_CurrentWeaponType; };
	//E_WEAPON_TYPE GetLastWeaponType() { return e_CurrentWeaponType; };

	void LoadSkeletalMeshType(E_WEAPON_TYPE e_WeaponType);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	class UWeaponLineTrace* LineTrace;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AUDIO")
	UAudioComponent* AudioComponent;

	/** Location on gun mesh where projectiles should spawn. */
	/*UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* MuzzleLocation;*/

	/** Gun Spawn offset from the Weapon location */
	UPROPERTY(EditAnywhere)
	FVector SpawnOffset;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere)
	class UAnimMontage* FireAnimation;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	UFUNCTION()
	void OnFire(ATPSCharacter* Character);

	float GetFireRate() { return FireRate; };
	void SetFireRate(float setFireRate) { FireRate = setFireRate; };

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
