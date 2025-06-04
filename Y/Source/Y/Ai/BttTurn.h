// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BttTurn.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UBttTurn : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBttTurn();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	
};
