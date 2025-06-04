// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "yHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UyHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()

public:
	UyHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	/*FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp;	}*/
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;



private:


	
};
