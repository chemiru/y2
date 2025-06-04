// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/yCh_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Data/yCh_DataAsset.h"
#include "Ui/yHudWidget.h"
#include "CharacterStat/yCharacterStatComponent.h"

AyCh_Player::AyCh_Player()
{

	//Camera Section
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Input
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputJumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputJumpActionRef.Object)
	{
		JumpAction = InputJumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ControlChange.IA_ControlChange'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ControlChangeAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShuolderMove.IA_ShuolderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShoulderLookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShuolderLook.IA_ShuolderLook'"));
	if (nullptr != ShoulderLookActionRef.Object)
	{
		ShoulderLookAction = ShoulderLookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuaterMoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != QuaterMoveActionRef.Object)
	{
		QuaterMoveAction = QuaterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	CurrentCharacterDataType = ECharacterControlDataType::Quater;

}

void AyCh_Player::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterDataType);
}

void AyCh_Player::SetDead()
{
	Super:: SetDead();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}


void AyCh_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ControlChangeAction, ETriggerEvent::Triggered, this, &AyCh_Player::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AyCh_Player::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AyCh_Player::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AyCh_Player::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AyCh_Player::Attack);

}

void AyCh_Player::ChangeCharacterControl()
{
	if (CurrentCharacterDataType == ECharacterControlDataType::Quater)
	{
		SetCharacterControl(ECharacterControlDataType::Shuolder);
	
	}

	else if (CurrentCharacterDataType == ECharacterControlDataType::Shuolder)
	{
		SetCharacterControl(ECharacterControlDataType::Quater);
	}
}

void AyCh_Player::SetCharacterControl(ECharacterControlDataType InDataType)
{
	UyCh_DataAsset* NewCharacterControl = CharacterControlTMap[InDataType];
	check(NewCharacterControl);
	
	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}

	}

	CurrentCharacterDataType = InDataType;

}

void AyCh_Player::SetCharacterControlData(const UyCh_DataAsset* CharacterControlDataAsset)
{
	Super::SetCharacterControlData(CharacterControlDataAsset);

	CameraBoom->TargetArmLength = CharacterControlDataAsset->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlDataAsset->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlDataAsset->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlDataAsset->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlDataAsset->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlDataAsset->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlDataAsset->bDoCollisionTest;

}

void AyCh_Player::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// 컨트롤러 회전값 가져오기
		const FRotator ControlRotation = Controller->GetControlRotation();

		// 현재 회전값에서 전방 및 우측 벡터 가져오기
		const FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
		const FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);

		// 입력값을 방향 벡터에 적용하여 이동
		AddMovementInput(ForwardVector, MovementVector.X);
		AddMovementInput(RightVector, MovementVector.Y);
	}
}
	/*FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);*/


void AyCh_Player::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AyCh_Player::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// 컨트롤러 회전값 가져오기
		const FRotator ControlRotation = Controller->GetControlRotation();

		// 현재 회전값에서 전방 및 우측 벡터 가져오기
		const FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y);
		const FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);

		// 입력값을 방향 벡터에 적용하여 이동
		AddMovementInput(ForwardVector, MovementVector.X);
		AddMovementInput(RightVector, MovementVector.Y);
	}

}

void AyCh_Player::Attack()
{
	ProcessComboCommand();
}

void AyCh_Player::SetupHudWidget(UyHudWidget* InHudWidget)
{
	if (InHudWidget)
	{
		InHudWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHudWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnStatChanged.AddUObject(InHudWidget, &UyHudWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHudWidget, &UyHudWidget::UpdateHpBar);

	}
}

