// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDefines.h"
#include "Weapons.generated.h"


UCLASS()
class TPS_API AWeapons : public AActor
{
	GENERATED_BODY()


public:	

	// Sets default values for this actor's properties
	AWeapons();

	E_WEAPON_TYPE mWeponType;

	void SetWeaponType(E_WEAPON_TYPE e_WeaponType) { mWeponType = e_WeaponType;  };

	//E_WEAPON_TYPE GetCurrWeaponType() { return e_CurrentWeaponType; };
	//E_WEAPON_TYPE GetLastWeaponType() { return e_CurrentWeaponType; };

	void LoadSkeletalMeshType(E_WEAPON_TYPE e_WeaponType);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
