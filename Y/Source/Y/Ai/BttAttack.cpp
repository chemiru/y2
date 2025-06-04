// Fill out your copyright notice in the Description page of Project Settings.


#include "BttAttack.h"
#include "CrowdAIController.h"
#include "Interface/AiCharacterInterface.h"
//#include "Character/yNpc.h"


UBttAttack::UBttAttack()
{
}

EBTNodeResult::Type UBttAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;

	}

	IAiCharacterInterface* AIPawn = Cast< IAiCharacterInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAiAttackDelegate OnAttackFinished;
	OnAttackFinished.BindLambda([&]() 
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAiAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAi();

	return EBTNodeResult::InProgress;

	//EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	//if (nullptr == ControllingPawn)
	//{
	//	return EBTNodeResult::Failed;

	//}

	//IAiCharacterInterface* AIPawn = Cast< IAiCharacterInterface>(ControllingPawn);
	//if (nullptr == AIPawn)
	//{
	//	return EBTNodeResult::Failed;
	//}

	//AyNpc* Npc = Cast< AyNpc>(ControllingPawn);
	////FAiAttackDelegate OnAttackFinished;
	//Npc->OnAttackFinished.BindLambda([&]()
	//	{
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	}
	//);

	///*Npc->SetAiAttackDelegate(OnAttackFinished);*/
	//AIPawn->AttackByAi();

	//return EBTNodeResult::InProgress;

	
}
