// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UySaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RetryCount;
	
};
