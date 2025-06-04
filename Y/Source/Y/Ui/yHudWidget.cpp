// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/yHudWidget.h"
#include "Ui/yHpBarWidget.h"
#include "Ui/CharacterStatWidget.h"
#include "Interface/CharacterHudInterface.h"

UyHudWidget::UyHudWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

void UyHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UyHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	ICharacterHudInterface* HudPawn = Cast<ICharacterHudInterface>(GetOwningPlayerPawn());
	if (HudPawn)
	{
		HudPawn->SetupHudWidget(this);
	}
}

void UyHudWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	FABCharacterStat TotalStat = BaseStat + ModifierStat;
	//HpBar->SetMaxHp(TotalStat.MaxHp);
	HpBar->UpdateStat(BaseStat, ModifierStat);
	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UyHudWidget::UpdateHpBar(float InCurrentHp)
{
	HpBar->UpdateHpBar(InCurrentHp);
	
}
