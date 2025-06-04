// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BtsDetect.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UBtsDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBtsDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
