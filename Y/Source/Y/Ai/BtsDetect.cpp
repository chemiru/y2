// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/BtsDetect.h"
#include "ABAi.h"
#include "CrowdAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AiCharacterInterface.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
//#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
//#include "CollisionQueryParams.h"


UBtsDetect::UBtsDetect()
{
	//NodeName = TEXT("Detect2");
	Interval = 1.0f;
}

void UBtsDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IAiCharacterInterface* AiPawn = Cast<IAiCharacterInterface>(ControllingPawn);
	if (nullptr == AiPawn)
	{
		return;
	}

	float DetectRadius = AiPawn->GetAiDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults, Center, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParam);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;

			}

		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

}
