// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/yGameMode.h"
#include "Player/yPlayerController.h"

AyGameMode::AyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Y.yCh_Player")); 
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;

	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Y.yPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;

	}

	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}
void AyGameMode::OnPlayerScoreChanged(int32 Score)
{
	CurrentScore = Score;

	AyPlayerController* PlayerController = Cast<AyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GameScoreChanged(CurrentScore);
	}

	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;

		if (PlayerController)
		{
			PlayerController->GameCleared();
		}
	
	}
	
}

void AyGameMode::OnPlayerDead()
{
	AyPlayerController* PlayerController = Cast<AyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GameOver();
	}
}

bool AyGameMode::IsGameCleared()
{
	return bIsCleared;
}
;
	
