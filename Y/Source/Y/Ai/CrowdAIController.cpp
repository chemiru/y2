// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/CrowdAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ABAi.h"


ACrowdAIController::ACrowdAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BbRef(TEXT("/ Script / AIModule.BlackboardData'/Game/ArenaBattle/AI/BbCharacter.BbCharacter'"));
	if (nullptr != BbRef.Object)
	{
		BbAsset = BbRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BtRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ArenaBattle/AI/BtCharacter.BtCharacter'"));
	if (nullptr != BtRef.Object)
	{
		BtAsset = BtRef.Object;

	}
	
}

void ACrowdAIController::RunAi()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BbAsset, BlackboardPtr))
	{
		//Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
		//Blackboard->SetValueAsVector( TEXT("HomePos"), GetPawn()->GetActorLocation());
		BlackboardPtr->SetValueAsVector(TEXT("HomePos"), GetPawn()->GetActorLocation());
		
		bool RunResult = RunBehaviorTree(BtAsset);
		ensure(RunResult);
	}
}

void ACrowdAIController::StopAi()
{
	UBehaviorTreeComponent* BtComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BtComponent)
	{
		BtComponent->StopTree();
	}

}

void ACrowdAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAi();

}
