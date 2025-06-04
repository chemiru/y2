// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "yCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMaxHpDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FABCharacterStat& /*BaseStat*/, const FABCharacterStat& /*ModifierStat*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class Y_API UyCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UyCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	FOnMaxHpDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InnewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void AddBaseStat(const FABCharacterStat& InBaseStat) {
		//BaseStat += InBaseStat(x) is not allowed, so we use the operator+= defined in FABCharacterStat
		BaseStat = BaseStat + InBaseStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetBaseStat(const FABCharacterStat& InBaseStat) {
		BaseStat = InBaseStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat) {
		ModifierStat = InModifierStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE void HealHp(float InHealAmount)
	{
		CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0.0f, GetTotalStat().MaxHp);
		OnHpChanged.Broadcast(CurrentHp);
	}
	FORCEINLINE float GetAttackRadius() { return AttackRadius; }
	float ApplyDamage(float InDamage);

//public:	
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	void SetHp(float NewHp);

	/*UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;*/

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FABCharacterStat ModifierStat;

};
