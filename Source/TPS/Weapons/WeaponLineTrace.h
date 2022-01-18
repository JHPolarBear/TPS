// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UWeaponLineTrace : public UObject
{
	GENERATED_BODY()

public:
	void OnFire(class ATPSCharacter* Character );
	
};
