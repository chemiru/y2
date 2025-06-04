// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/CharacterStatWidget.h"
#include "Components/TextBlock.h"

void UCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}

}


void UCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*) & BaseStat, &BaseData);
		
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*) & ModifierStat, &ModifierData);

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
	
	
	/* "The following method is better for performance."
	for (const auto& Pair : BaseLookup)
	{
		const FName PropKey = Pair.Key;
		UTextBlock* BaseTextBlock = Pair.Value;
		if (BaseTextBlock)
		{
			const float BaseValue = BaseStat.GetValue(PropKey);
			BaseTextBlock->SetText(FText::AsNumber(BaseValue));
		}
	}
	


	for (const auto& Pair : ModifierLookup)
	{
		const FName PropKey = Pair.Key;
		UTextBlock* ModifierTextBlock = Pair.Value;
		if (ModifierTextBlock)
		{
			const float ModifierValue = ModifierStat.GetValue(PropKey);
			ModifierTextBlock->SetText(FText::AsNumber(ModifierValue));
		}
	}

	FABCharacterStat.h
	float GetValue(const FName& PropKey) const
	{
		if (PropKey == "MaxHp") return MaxHp;
		if (PropKey == "Attack") return Attack;
		if (PropKey == "AttackRange") return AttackRange;
		if (PropKey == "AttackSpeed") return AttackSpeed;
		if (PropKey == "MovementSpeed") return MovementSpeed;
		return 0.0f;
	}*/
}


