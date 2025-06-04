// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BttFindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UBttFindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBttFindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
