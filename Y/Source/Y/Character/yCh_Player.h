// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/yCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/CharacterHudInterface.h"
#include "yCh_Player.generated.h"


/**
 * 
 */
UCLASS()
class Y_API AyCh_Player : public AyCharacterBase, public ICharacterHudInterface
{
	GENERATED_BODY()

public:
	AyCh_Player();

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//CharacterControlSection
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlDataType InDataType);
	virtual void SetCharacterControlData(const class UyCh_DataAsset* CharacterControlDataAsset) override;


	//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	//Input Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ControlChangeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlDataType CurrentCharacterDataType;

	void Attack();

	//UI Section
protected:
	virtual void SetupHudWidget(class UyHudWidget* InHudWidget) override;
};
