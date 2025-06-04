// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "GameData/ABCharacterStat.h"
#include "yCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCharacterBase, Log, All);

UENUM()
enum class ECharacterControlDataType : uint8
{
	Shuolder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData*);

USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()

	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate){}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class Y_API AyCharacterBase : public ACharacter, public IABAnimationAttackInterface, public IABCharacterWidgetInterface, public IABCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AyCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UyCh_DataAsset* CharacterControlDataAsset);

	UPROPERTY(EditAnywhere, Category = CharacterControlData, Meta = (AllowPrivateAccess = "true"))
	TMap< ECharacterControlDataType, class UyCh_DataAsset*> CharacterControlTMap;

	//ComboAction Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess= "true"))
	TObjectPtr<class UyComboDataAsset> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();


	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

	//Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//DeadSection
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

	//StatSection
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UyCharacterStatComponent> Stat;

	//UIWidget Section
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;
	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override;

	//Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;
	


	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UABItemData* InItemData) override;
	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

	//Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNew);
	void ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);

};
