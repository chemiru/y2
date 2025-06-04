// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AiCharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAiCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAiAttackDelegate);

/**
 * 
 */

class Y_API IAiCharacterInterface
{
	GENERATED_BODY()

public:
	virtual float GetAiPatrolRadius() = 0;
	virtual float GetAiDetectRange() = 0;
	virtual float GetAiAttackRange() = 0;
	virtual float GetAiTurnSpeed() = 0;

	virtual void SetAiAttackDelegate(FAiAttackDelegate InOnAttackFinished) = 0;
	virtual void AttackByAi() = 0;

};
