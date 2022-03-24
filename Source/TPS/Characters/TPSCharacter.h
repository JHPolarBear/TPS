// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CharacterDefines.h"
#include "CommonDefines.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

UCLASS(config=Game)
class ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ATPSCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	AWeapons* GetWeapon() { return Weapon; };

	float FireDeltaTime;

	///** Whether to use motion controller location for aiming. */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//bool bUsingMotionControllers;
	void WeaponEquip(E_WEAPON_TYPE e_CurrentWeaponType);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	bool IsBattleMode;
	FORCEINLINE bool GetIsBattleMode() const { return IsBattleMode; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	bool IsDeath;
	FORCEINLINE bool GetIsDeath() const { return IsDeath; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim, meta = (AllowPrivateAccess = "true"))
	bool IsFiring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Monster)
	bool IsMonster;

	void StartDeathAction();

	void SetIsFiring(bool setFiring);

	FORCEINLINE bool GetIsFiring() const { return IsFiring; }

	ATPSPlayerState* GetTPSPlayerState() { return TPSPlayerState; }

	ETeamAttitude::Type CompareTeamAttribute(const ATPSCharacter& OtherCharacter);

	int GetCurrentBulletNum();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:

	UPROPERTY(VisibleAnywhere)
	class AWeapons* Weapon;

	UFUNCTION()
	virtual void OnFire();
	UFUNCTION()
	virtual void OnFireStop();

	bool IsSprint;
	UFUNCTION()
	virtual void OnSprint();
	UFUNCTION()
	virtual void OnSprintStop();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim)
	class UTPSAnimInstance* TPSAnimInstance;

	void SetControlMode(E_CONTROL_MODE NewControlMode);
	E_CONTROL_MODE CurrentColtrolMode = E_CONTROL_MODE::NORMAL;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLegthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLegthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	void Aim();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	FTimerHandle ReloadTimer;
	void OnReloadingStart();
	void OnReloadingEnd();

	UPROPERTY()
	class ATPSPlayerState* TPSPlayerState;

	UPROPERTY()
	class ATPSPlayerController* TPSPlayerController;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

