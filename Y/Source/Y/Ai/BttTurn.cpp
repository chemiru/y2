// Fill out your copyright notice in the Description page of Project Settings.


#include "BttTurn.h"
#include "CrowdAIController.h"
#include "Interface/AiCharacterInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAi.h"

UBttTurn::UBttTurn()
{
}

EBTNodeResult::Type UBttTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	IAiCharacterInterface* AIPawn = Cast<IAiCharacterInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	float TurnSpeed = AIPawn->GetAiTurnSpeed();
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	
	return EBTNodeResult::Succeeded;
}
