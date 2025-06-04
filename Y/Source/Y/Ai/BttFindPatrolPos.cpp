// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/BttFindPatrolPos.h"
#include "ABAi.h"
#include "CrowdAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AiCharacterInterface.h"

UBttFindPatrolPos::UBttFindPatrolPos()
{
}

EBTNodeResult::Type UBttFindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	IAiCharacterInterface* AiPawn = Cast<IAiCharacterInterface>(ControllingPawn);
	if (nullptr == AiPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	float PatrolRadius = AiPawn->GetAiPatrolRadius();
	FNavLocation NextPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextPatrolPos))
	{
		//OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), NextPatrolPos.Location);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;


}
