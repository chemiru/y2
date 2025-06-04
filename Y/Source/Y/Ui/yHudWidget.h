// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "yHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UyHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UyHudWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float InCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UyHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UCharacterStatWidget> CharacterStat;

};
