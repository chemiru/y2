// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/BtdAttackRange.h"
//#include "BtdAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CrowdAIController.h"
#include "ABAi.h"
#include "Interface/AiCharacterInterface.h"


UBtdAttackRange::UBtdAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBtdAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IAiCharacterInterface* AiPawn = Cast<IAiCharacterInterface>(ControllingPawn);
	if (nullptr == AiPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == Target)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AiPawn->GetAiAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);

	return bResult;

}
