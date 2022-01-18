// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/Rifle.h"

//////////////////////////////////////////////////////////////////////////
// ATPSCharacter

ATPSCharacter::ATPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeLocation(FVector(70.0f, 70.0f, 70.0f));
	FollowCamera->SetRelativeRotation(FRotator(0.0f,355.0f,0.0f));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MESH(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MESH.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Request Test
	//static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_BP(TEXT("AnimBlueprint'/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint.UE4ASP_HeroTPP_AnimBlueprint_C'"));
	// TPS ANIM
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_BP(TEXT("AnimBlueprint'/Game/AnimStarterPack/UE4_Mannequin/TPS_AnimBlueprint.TPS_AnimBlueprint_C'"));
	
	if (ANIM_BP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIM_BP.Class);
	}

	FireDeltaTime = 0.0f;

	ArmLegthSpeed = 25.0f;

	IsDeath = false;

	nMaxHealth = 100;
	nCurrentHealth = nMaxHealth;

}
void ATPSCharacter::WeaponEquip(E_WEAPON_TYPE e_CurrentWeaponType)
{
	FName WeaponSocket;
	
	switch (e_CurrentWeaponType)
	{
	case E_RIFLE:
		WeaponSocket = TEXT("RifleWeaponSocket");
		break;
	case E_PISTOL:
		WeaponSocket = TEXT("PistolWeaponSocket");
		break;
	default:
		WeaponSocket = TEXT("RifleWeaponSocket");
		break;
	}

	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		UWorld* world = GetWorld();
		if (world)
		{
			auto NewWeapon = world->SpawnActor<ARifle>(ARifle::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	
			if (nullptr != NewWeapon)
			{
				Weapon = NewWeapon;
				Weapon->SetActorHiddenInGame(false);
				Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
				//LOG_WARNING(TEXT("Weapon Aattch!"));
				Weapon->GetAttachParentSocketName();
			}
		}
	}
}
void ATPSCharacter::SetControlMode(E_CONTROL_MODE NewControlMode)
{
	CurrentColtrolMode = NewControlMode;

	if (CurrentColtrolMode == E_CONTROL_MODE::NORMAL)
	{
		//ArmLegthTo = 300.0f;
		IsBattleMode = false;
		ArmLegthTo = 90.0f;

	}
	else if (CurrentColtrolMode == E_CONTROL_MODE::BATTLE)
	{
		//ArmLegthTo = 55.0f;
		IsBattleMode = true;
		ArmLegthTo = 30.0f;
	}

	if (TPSAnimInstance != nullptr)
	{
		TPSAnimInstance->SetState((int)CurrentColtrolMode);
	}

}

void ATPSCharacter::OnFireStop()
{
	LOG_WARNING(TEXT("On Stop!!!"));

	SetIsFiring(false);

	FireDeltaTime = 0;
}
void ATPSCharacter::OnFire()
{
	LOG_WARNING(TEXT("On Fire!!!"));
	
	SetIsFiring(true);

	// 첫발을 위해서 초기 셋팅
	FireDeltaTime = Weapon->GetFireRate();
}

void ATPSCharacter::SetIsFiring(bool setFiring)
{
	IsFiring = setFiring;
	
	if (TPSAnimInstance != nullptr)
		TPSAnimInstance->SetIsAttack(IsFiring);
}
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();


	// 소켓에 총 붙이기
	WeaponEquip(E_RIFLE);


	TPSAnimInstance = Cast<UTPSAnimInstance>(GetMesh()->GetAnimInstance());

	SetControlMode(E_CONTROL_MODE::NORMAL);


}

void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 카메라 암 이동 방법
	//CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ArmLegthTo, DeltaTime, ArmLegthSpeed);
	
	// 카메라 필드오브뷰 축소방법
	FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, ArmLegthTo, DeltaTime, ArmLegthSpeed);

	if (GetIsFiring())
	{
		if (FireDeltaTime >= Weapon->GetFireRate())
		{
			if (Weapon)
			{
				Weapon->OnFire(this);
			}
			FireDeltaTime = 0.0f;
			// 단발 총(권총)인 경우에는 연발 안되게처리
			if (!Weapon->bFullAutoFire)
			{
				SetIsFiring(false);
			}
		}
		else
		{
			FireDeltaTime += DeltaTime;
		}
	}
		
	/*switch (CurrentColtrolMode)
	{
	case EControlMode::TPS:
	case EControlMode::ZOOM:
		CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}*/
}


//////////////////////////////////////////////////////////////////////////
// Input
// 
// Called when the game starts or when spawned

void ATPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &ATPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &ATPSCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATPSCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATPSCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATPSCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATPSCharacter::Aim);

	// Fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSCharacter::OnFireStop);

	PlayerInputComponent->BindAction("Reloading", IE_Released, this, &ATPSCharacter::OnReloadingStart);
}
void ATPSCharacter::OnReloadingStart()
{
	LOG_WARNING(TEXT("OnReloadingStart!!!"));

	// 장전중 다시누르면 장전 취소
	if(Weapon->bIsReloading)
	{
		Weapon->bIsReloading = false;
		TPSAnimInstance->SetIsReloading(Weapon->bIsReloading);

		GetWorldTimerManager().ClearTimer(ReloadTimer);
		LOG_WARNING(TEXT("OnReloadingStart Cancel!!!"));
	}
	// 장전시작
	else
	{
		Weapon->bIsReloading = true;
		TPSAnimInstance->SetIsReloading(Weapon->bIsReloading);

		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ATPSCharacter::OnReloadingEnd, 2, false);
		LOG_WARNING(TEXT("OnReloadingStart2!!!"));
	}
}

void ATPSCharacter::OnReloadingEnd() 
{
	// 호출됐을때 장전중이었으면 탄창 최대로 채워주고 장전 액션 종료.
	if (Weapon->bIsReloading)
	{
		Weapon->bIsReloading = false;
		TPSAnimInstance->SetIsReloading(Weapon->bIsReloading);
		Weapon->nCurrentBulletNum = Weapon->nMaxBulletNum;
		LOG_WARNING(TEXT("OnReloadingEnd!!!"));
	}
}

void ATPSCharacter::Aim()
{
	switch (CurrentColtrolMode)
	{
	case E_CONTROL_MODE::NORMAL:
		SetControlMode(E_CONTROL_MODE::BATTLE);
		break;
	case E_CONTROL_MODE::BATTLE:
		SetControlMode(E_CONTROL_MODE::NORMAL);
		break;
	}

}

void ATPSCharacter::OnResetVR()
{
	// If TPS is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in TPS.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATPSCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ATPSCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ATPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATPSCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATPSCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
