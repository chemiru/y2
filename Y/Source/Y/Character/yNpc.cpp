// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/yNpc.h"
#include "Engine/AssetManager.h"
#include "Ai/CrowdAIController.h"
#include "CharacterStat/yCharacterStatComponent.h"

AyNpc::AyNpc()
{
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = ACrowdAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AyNpc::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AyNpc::NPCMeshLoadComplete));

}

void AyNpc::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
		FTimerDelegate::CreateLambda([&]() {Destroy();}), 
		DeadEventDelayTime, false);

	ACrowdAIController* AiController = Cast<ACrowdAIController>(GetController());

	if (AiController)
	{
		AiController->StopAi();
	}

}

void AyNpc::NPCMeshLoadComplete()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast< USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);

		}

	}

	NPCMeshHandle->ReleaseHandle();
}

float AyNpc::GetAiPatrolRadius()
{
	return 800.0f;
}

float AyNpc::GetAiDetectRange()
{
	return 400.0f;
}

float AyNpc::GetAiAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius()*2;
}

float AyNpc::GetAiTurnSpeed()
{
	return 2.0f;
}

void AyNpc::SetAiAttackDelegate(FAiAttackDelegate InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AyNpc::AttackByAi()
{
	ProcessComboCommand();
}

void AyNpc::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();

}



