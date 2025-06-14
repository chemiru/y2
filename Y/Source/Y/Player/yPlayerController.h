﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "yPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogyPlayerController, Log, All);
/**
 * 
 */
UCLASS()
class Y_API AyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AyPlayerController();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "K2_OnScoreChanged"))
	void K2_OnScoreChanged(int32 Score);
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "K2_OnGameClear"))
	void K2_OnGameClear();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "K2_OnGameOver"))
	void K2_OnGameOver();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "K2_OnGameRetry"))
	void K2_OnGameRetry(int32 NewRetryCount);


	void GameScoreChanged(int32 Score);
	void GameCleared();
	void GameOver();
	
protected:
	virtual void BeginPlay() override;


// Hud Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hud)
	TSubclassOf<class UyHudWidget> yHudWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hud)
	TObjectPtr<class UyHudWidget> yHudWidget;
	
	// Save Game Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UySaveGame> SaveGameInstance;

};
