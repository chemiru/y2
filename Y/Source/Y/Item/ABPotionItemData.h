// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "ABPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UABPotionItemData : public UABItemData
{
	GENERATED_BODY()

	

public:

	UABPotionItemData();
	

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());

	}

	UPROPERTY(EditAnywhere, Category = Potion)
	float HealAmount;
	
};
