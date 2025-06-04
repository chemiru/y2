// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/yCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"

// Sets default values for this component's properties
UyCharacterStatComponent::UyCharacterStatComponent()
{
	/*MaxHp = 200.0f;
	CurrentHp = MaxHp;*/

	CurrentLevel = 1;
	AttackRadius = 50.0f;
	/*CurrentHp = BaseStat.MaxHp;*/

	bWantsInitializeComponent = true;
}


void UyCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
	
}

// yCharacterStatComponent.cpp
void UyCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 원하는 초기화 코드 작성
}


// Called when the game starts
//void UyCharacterStatComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	/*SetHp(MaxHp);*/
//
//	/*SetLevelStat(CurrentLevel);
//	SetHp(BaseStat.MaxHp);*/
//	
//}

void UyCharacterStatComponent::SetLevelStat(int32 InnewLevel)
{
	CurrentLevel = FMath::Clamp(InnewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f);
}

float UyCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp<=KINDA_SMALL_NUMBER)
	{ 
		OnHpZero.Broadcast();
	}
	
	return ActualDamage;
}

void UyCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);

}




