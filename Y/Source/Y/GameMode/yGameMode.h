// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/YGameModeInterface.h"
#include "yGameMode.generated.h"

/**
 * 
 */
UCLASS()
class Y_API AyGameMode : public AGameModeBase, public IYGameModeInterface
{
	GENERATED_BODY()

public:
	AyGameMode();

public:
	virtual void OnPlayerScoreChanged(int32 Score) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;

	
};
