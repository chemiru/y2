// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "GameData/ABCharacterStat.h"
#include "ABScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UABScrollItemData : public UABItemData
{
	GENERATED_BODY()

public:

	UABScrollItemData();
	

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());

	}
	
public:
	UPROPERTY(EditAnywhere, Category = Scroll)
	FABCharacterStat BaseStat;
};
