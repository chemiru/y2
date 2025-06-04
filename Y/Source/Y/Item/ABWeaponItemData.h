// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "GameData/ABCharacterStat.h"
#include "ABWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class Y_API UABWeaponItemData : public UABItemData
{
	GENERATED_BODY()

public:
	UABWeaponItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());

	}
		

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
	/*TObjectPtr<USkeletalMesh> WeaponMesh;
	*/

	UPROPERTY(EditAnywhere, Category = Weapon)
	FABCharacterStat ModifierStat;
};
