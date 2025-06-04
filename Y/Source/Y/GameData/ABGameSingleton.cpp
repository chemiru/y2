// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatRef(TEXT("/Script/Engine.DataTable'/Game/GameData/DtABCharacteStat.DtABCharacteStat'"));
	if (nullptr != CharacterStatRef.Object)
	{
		const UDataTable* DataTable = CharacterStatRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(
			ValueArray, CharacterStatTable, [](uint8* Value) 
			{ 
				return *(FABCharacterStat*)Value; 
			}
		);

		/*TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(
			ValueArray, CharacterStatTable, [](uint8* Value) 
			{ 
				return *(FABCharacterStat*)Value; 
			}
		);
		=
		TArray<FName> RowNames = DataTable->GetRowNames();
		for (const FName& Name : RowNames)
		{
			if (const FABCharacterStat* Stat = DataTable->FindRow<FABCharacterStat>(Name, TEXT("")))
			{
				CharacterStatTable.Add(*Stat);
			}
		}*/


	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(0 < CharacterMaxLevel);

}

UABGameSingleton& UABGameSingleton::Get()
{
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);//for auto load
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogABGameSingleton, Error, TEXT("UABGameSingleton::Get() - Singleton is nullptr"));
	
	//return *NewObject<UABGameSingleton>();

	static UABGameSingleton* StaticFallbackInstance = []() 
		{
		UABGameSingleton* NewSingleton = NewObject<UABGameSingleton>();
		NewSingleton->AddToRoot(); // GC ¹æÁö
		return NewSingleton;
		}();

	return *StaticFallbackInstance;

}
