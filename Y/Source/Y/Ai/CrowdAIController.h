// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "CrowdAIController.generated.h"

/**
 * 
 */
UCLASS()
class Y_API ACrowdAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	ACrowdAIController();

	void RunAi();
	void StopAi();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BbAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BtAsset;

		
};
