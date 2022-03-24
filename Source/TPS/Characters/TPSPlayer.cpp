// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/TPSPlayer.h"

ATPSPlayer::ATPSPlayer()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetRelativeLocation(FVector(70.0f, 70.0f, 70.0f));
	FollowCamera->SetRelativeRotation(FRotator(0.0f, 355.0f, 0.0f));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	IsMonster = false;
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//LOG_WARNING(TEXT("Character Controller Setting start"));

	// ��Ʈ�ѷ� ����
	TPSPlayerController = Cast<ATPSPlayerController>(GetController());
	ASSERT_CHECK(TPSPlayerController != nullptr);

	if (TPSPlayerController)
	{
		// �÷��̾� ������Ʈ ����
		TPSPlayerState = TPSPlayerController->GetPlayerState<ATPSPlayerState>();
		ASSERT_CHECK(TPSPlayerState != nullptr);

		TPSPlayerState->SetDefaultWalkSpeed(GetCharacterMovement()->MaxWalkSpeed);
		TPSPlayerState->SetRunMultiplier(5.0f);

		TPSPlayerController->SettingWidget();

		TPSPlayerState->SetMaxBulletCount(Weapon->GetMaxBulletNum());
		TPSPlayerState->SetBulletCount(GetCurrentBulletNum());
	}

	// EQS���� �÷��̾� ���⿡ ����� �÷��̾� �±� �߰�
	Tags.Add(FName("Player"));

	//LOG_WARNING(TEXT("Character Controller Setting end"));
}

void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ī�޶� �� �̵� ���
	//CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ArmLegthTo, DeltaTime, ArmLegthSpeed);


	// ī�޶� �ʵ����� ��ҹ��
	FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, ArmLegthTo, DeltaTime, ArmLegthSpeed);

	/*switch (CurrentColtrolMode)
	{
	case EControlMode::TPS:
	case EControlMode::ZOOM:
		CameraBoom->SetRelativeRotation(FMath::RInterpTo(CameraBoom->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}*/
}
