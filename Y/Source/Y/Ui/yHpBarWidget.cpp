// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/yHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/ABCharacterWidgetInterface.h"

UyHpBarWidget::UyHpBarWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UyHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
	
}

void UyHpBarWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	MaxHp = BaseStat.MaxHp + ModifierStat.MaxHp;
	ensure(MaxHp > 0.0f);

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UyHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;
	
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp/MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
	
}

FString UyHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%.0f"), CurrentHp, MaxHp);
}

