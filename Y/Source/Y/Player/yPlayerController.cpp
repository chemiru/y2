// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/yPlayerController.h"
#include "Ui/yHudWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ySaveGame.h"

DEFINE_LOG_CATEGORY(LogyPlayerController);


AyPlayerController::AyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UyHudWidget> yHudRef(TEXT("/Game/ArenaBattle/UI/Wbp_yHud.Wbp_yHud_C"));
	if (yHudRef.Class)
	{
		yHudWidgetClass = yHudRef.Class;
	}
}

void AyPlayerController::GameScoreChanged(int32 Score)
{
	K2_OnScoreChanged(Score);
}

void AyPlayerController::GameCleared()
{
	K2_OnGameClear();
}

void AyPlayerController::GameOver()
{
	K2_OnGameOver();

	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogyPlayerController, Error, TEXT("Failed to save game!"));
	}

	K2_OnGameRetry(SaveGameInstance->RetryCount);
	
}

void AyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	/*yHudWidget = CreateWidget<UyHudWidget>(this, yHudWidgetClass);
	if (yHudWidget)
	{
		yHudWidget->AddToViewport();
	}*/

	SaveGameInstance = Cast<UySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		SaveGameInstance->RetryCount++;
	}
	else
	{
		SaveGameInstance = NewObject<UySaveGame>();// savegameInstance가 없다면 새로운 객체를 생성하는 거임
		SaveGameInstance->RetryCount = 0; // 처음 시작할 때는 RetryCount를 0으로 초기화
	}

	K2_OnGameRetry(SaveGameInstance->RetryCount);
	
}
