// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

// Sets default values
ARifle::ARifle()
{
	SetWeaponType(E_RIFLE);
	LoadSkeletalMeshType(mWeponType);
	bFullAutoFire = true;

	nMaxBulletNum = 30;
	nCurrentBulletNum = nMaxBulletNum;
	FireRate = 0.2f;
	ThumbnailPath = TEXT("Texture2D'/Game/Textures/ak47.ak47'");
}

// Called when the game starts or when spawned
void ARifle::BeginPlay()
{
	Super::BeginPlay();
	SetFireRate(0.1f);
	
}

// Called every frame
void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

