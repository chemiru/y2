// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/StageGimmick.h"
#include "StageGimmick.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/ABCollision.h"
#include "Character/yNpc.h"
#include "Engine/World.h" 
#include "Engine/EngineTypes.h" 
#include "CollisionQueryParams.h" 
#include "WorldCollision.h" // FOverlapResult 
#include "Engine/OverlapResult.h"
#include "Item/ABItemBox.h"
#include "Interface/YGameModeInterface.h"

// Sets default values
AStageGimmick::AStageGimmick()
{
    // Stage Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshAsset.Object)
	{
		Stage->SetStaticMesh(StageMeshAsset.Object);
				
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AStageGimmick::OnStageTriggerBeginOverlap);



	// Gate Section

	static FName ArrGateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate")};
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	for (FName GateSocket : ArrGateSockets)
	{
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		Gate->SetStaticMesh(GateMeshRef.Object);
		Gate->SetupAttachment(Stage, GateSocket);
		Gate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		MapGates.Add(GateSocket, Gate);

		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		/*FString TriggerNameStr = GateSocket.ToString() + TEXT("Trigger");
		FName TriggerName=FName(TriggerNameStr);*/
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AStageGimmick::OnGateTriggerBeginOverlap);
		GateTrigger->ComponentTags.Add(GateSocket);

		ArrGateTriggers.Add(GateTrigger);
	}

	// Stage State
	CurrentState = EStageState::Ready;
	MapStateChangeActions.Add(EStageState::Ready, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this,&AStageGimmick::SetReady)));
	MapStateChangeActions.Add(EStageState::Fight, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AStageGimmick::SetFight)));
	MapStateChangeActions.Add(EStageState::Reward, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AStageGimmick::SetChooseReward)));
	MapStateChangeActions.Add(EStageState::Next, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AStageGimmick::SetChooseNext)));

	//Fight Section
	OpponentSpawnTime = 2.0f;
	OpponentClass = AyNpc::StaticClass();

	//Reward Section
	RewardBoxClass = AABItemBox::StaticClass();
	for (FName GateSocket : ArrGateSockets)
	{
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2.0f;
		MapBoxLocations.Add(GateSocket, BoxLocation);
	}

	//Stage Stat
	CurrentStageNum = 0;

}

void AStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);

}

void AStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::Fight);

}

void AStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(OverlappedComponent->ComponentTags.Num() ==1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	check(Stage->DoesSocketExist(SocketName));

	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);
	bool bResult = GetWorld()->OverlapMultiByObjectType(OverlapResults, NewLocation, FQuat::Identity, 
		FCollisionObjectQueryParams::AllStaticObjects, FCollisionShape::MakeSphere(775.0f), CollisionQueryParam);

	if (!bResult)
	{
		FTransform NewTransform(NewLocation);
		AStageGimmick* NewGimmick = GetWorld()->SpawnActorDeferred<AStageGimmick>(AStageGimmick::StaticClass(), NewTransform);
		if (NewGimmick)
		{
			NewGimmick->SetStageNum(CurrentStageNum + 1);
			NewGimmick->FinishSpawning(NewTransform);
		}
		
		/* before code NO Problom
		AStageGimmick* NewGimmick = GetWorld()->SpawnActor<AStageGimmick>(NewLocation, FRotator::ZeroRotator);
		if (NewGimmick)
		{
			NewGimmick->SetStageNum(CurrentStageNum + 1);
			
		}*/
		
		
		
		/* DefatulType
		
		AStageGimmick* SpawnedGimmick = GetWorld()->SpawnActor<AStageGimmick>(
			AStageGimmick::StaticClass(),
			NewLocation,
			FRotator::ZeroRotator,
			SpawnParams)*/
	

	}

}

void AStageGimmick::OpenAllGate()
{
	for (auto Gate : MapGates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	
}

void AStageGimmick::CloseAllGate()
{
	for (auto Gate : MapGates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}

void AStageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;

	if (MapStateChangeActions.Contains(CurrentState))
	{
		MapStateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
	
}

void AStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	for (auto GateTrigger : ArrGateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	OpenAllGate();
	
}

void AStageGimmick::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : ArrGateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	}
	
	CloseAllGate();

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);

}


void AStageGimmick::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : ArrGateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	}

	CloseAllGate();

	SpawnRewardBoxes();
}

void AStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : ArrGateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}
	OpenAllGate();
}

void AStageGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	IYGameModeInterface* GameModeInterface = Cast<IYGameModeInterface>(GetWorld()->GetAuthGameMode());
	if (GameModeInterface)
	{
		GameModeInterface->OnPlayerScoreChanged(CurrentStageNum);
		if (GameModeInterface->IsGameCleared())
		{
			return;
		}
	}
	SetState(EStageState::Reward);
}

void AStageGimmick::OnOpponentSpawn()
{
	/*const FVector SpawnLocation = GetActorLocation() + FVector::UpVector * 88.0f;
	AActor* OpponentActor = GetWorld()->SpawnActor(OpponentClass, &SpawnLocation, &FRotator::ZeroRotator);
	AyNpc* OpponentCharacter = Cast<AyNpc>(OpponentActor);
	if (OpponentCharacter)
	{
		OpponentCharacter->OnDestroyed.AddDynamic(this, &AStageGimmick::OnOpponentDestroyed);
		OpponentCharacter->SetLevel(CurrentStageNum);
	}*/

	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f);
	AyNpc* OpponentActor = GetWorld()->SpawnActorDeferred<AyNpc>(OpponentClass, SpawnTransform);
	if (OpponentActor)
	{
		OpponentActor->OnDestroyed.AddDynamic(this, &AStageGimmick::OnOpponentDestroyed);
		OpponentActor->SetLevel(CurrentStageNum);
		OpponentActor->FinishSpawning(SpawnTransform);
		
	}

}

void AStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : ArrRewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AABItemBox* ValidBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (ValidBox != OverlappedBox)
			{
				ValidBox->Destroy();
			}
		}
	}

	SetState(EStageState::Next);
}

void AStageGimmick::SpawnRewardBoxes()
{
	for (auto& BoxLocation : MapBoxLocations)
	{
		FTransform SpawnTransform(GetActorLocation() + BoxLocation.Value + FVector(0.0f, 0.0f, 30.0f));
		AABItemBox* RewardBoxActor = GetWorld()->SpawnActorDeferred<AABItemBox>(RewardBoxClass, SpawnTransform);
		
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(FName(BoxLocation.Key.ToString()));
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AStageGimmick::OnRewardTriggerBeginOverlap);

			ArrRewardBoxes.Add(RewardBoxActor);
		}
	}

	for (const auto& RewardBox : ArrRewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AABItemBox* ValidBox = RewardBox.Get();
			if (ValidBox)
			{
				ValidBox->FinishSpawning(ValidBox->GetActorTransform());
			}
		}
	}
	
	
	/*for (auto& BoxLocation : MapBoxLocations)
	{
		FVector WorldSpawnLocation = GetActorLocation() + BoxLocation.Value + FVector(0.0f, 0.0f, 30.0f);
		AActor* ItemActor = GetWorld()->SpawnActor(RewardBoxClass, &WorldSpawnLocation, &FRotator::ZeroRotator);
		AABItemBox* RewardBoxActor = Cast<AABItemBox>(ItemActor);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(FName(BoxLocation.Key.ToString()));
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AStageGimmick::OnRewardTriggerBeginOverlap);

			ArrRewardBoxes.Add(RewardBoxActor);
		}
	}*/
}



