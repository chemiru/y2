// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BttAttack.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UBttAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBttAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
