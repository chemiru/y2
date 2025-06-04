// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/yCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/AiCharacterInterface.h"
#include "yNpc.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FAiAttackDelegate);

UCLASS(config=y)
class Y_API AyNpc : public AyCharacterBase, public IAiCharacterInterface
{
	GENERATED_BODY()

public:
	AyNpc();

protected:
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void SetDead() override;

	void NPCMeshLoadComplete();

	UPROPERTY(config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;

	// Ai Section
protected:
	virtual float GetAiPatrolRadius() override;
	virtual float GetAiDetectRange() override;
	virtual float GetAiAttackRange() override;
	virtual float GetAiTurnSpeed() override;

	virtual void SetAiAttackDelegate(FAiAttackDelegate InOnAttackFinished) override;
public:
	//void SetAiAttackDelegate(FAiAttackDelegate InOnAttackFinished);
	virtual void AttackByAi() override;

	FAiAttackDelegate OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
