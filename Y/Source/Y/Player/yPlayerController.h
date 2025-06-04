// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "yPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class Y_API AyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AyPlayerController();
	
protected:
	virtual void BeginPlay() override;





// Hud Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hud)
	TSubclassOf<class UyHudWidget> yHudWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hud)
	TObjectPtr<class UyHudWidget> yHudWidget;
	




};
