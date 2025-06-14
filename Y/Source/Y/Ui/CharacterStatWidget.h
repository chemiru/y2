// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "CharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat ModifierStat);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*>BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;

	

};
