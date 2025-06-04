// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageGimmick.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate);

USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()

	FStageChangedDelegateWrapper(){	}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangedDelegate StageDelegate;

};

UENUM(BlueprintType)
enum class EStageState : uint8
{
	Ready = 0,
	Fight = 1,
	Reward = 2,
	Next = 3,
};

UCLASS()
class Y_API AStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageGimmick();

public:
	FORCEINLINE int32 GetStageNum() { return CurrentStageNum; }
	FORCEINLINE void SetStageNum(int32 InNum) {CurrentStageNum = InNum;	}

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Stage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Gete Section

protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> MapGates;

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> ArrGateTriggers;

	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenAllGate();
	void CloseAllGate();

	// Stage State
protected:
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	void SetState(EStageState InNewState);

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> MapStateChangeActions;

	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

	//Fight Section
protected:
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AyNpc> OpponentClass;

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);

	FTimerHandle OpponentTimerHandle;
	void OnOpponentSpawn();

	//Reward Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABItemBox> RewardBoxClass;
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AABItemBox>> ArrRewardBoxes;

	TMap<FName, FVector> MapBoxLocations;

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SpawnRewardBoxes();

	//Stage Stat
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentStageNum;


};
