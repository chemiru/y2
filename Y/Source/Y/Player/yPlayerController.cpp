// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/yPlayerController.h"
#include "Ui/yHudWidget.h"

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
}

void AyPlayerController::GameCleared()
{
}

void AyPlayerController::GameOver()
{
}

void AyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	yHudWidget = CreateWidget<UyHudWidget>(this, yHudWidgetClass);
	if (yHudWidget)
	{
		yHudWidget->AddToViewport();
	}

}
