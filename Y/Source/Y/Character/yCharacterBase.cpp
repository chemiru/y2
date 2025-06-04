// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/yCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/yCh_DataAsset.h"
#include "Animation/AnimMontage.h"
#include "yComboDataAsset.h"
#include "Physics/ABCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/yCharacterStatComponent.h"
#include "UI/ABWidgetComponent.h"
#include "UI/yHpBarWidget.h"
#include "Item/ABWeaponItemData.h"
#include "Item/ABItemData.h"
#include "UI/yHpBarWidget.h"
#include "Item/ABItems.h"


DEFINE_LOG_CATEGORY(LogCharacterBase);

// Sets default values
AyCharacterBase::AyCharacterBase()
{
    //Pawn
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    //Capsule
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

    //Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

    //Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef
    (TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh() -> SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef
    (TEXT("/Game/ArenaBattle/Animation/Abp_yCharacter.Abp_yCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    static ConstructorHelpers::FObjectFinder<UyCh_DataAsset> ShuolderRef(TEXT("/Script/Y.yCh_DataAsset'/Game/ArenaBattle/DataCharacterControl/Dty_Shouler.Dty_Shouler'"));
    if (ShuolderRef.Object)
    {
        CharacterControlTMap.Add(ECharacterControlDataType::Shuolder, ShuolderRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UyCh_DataAsset> QuaterRef(TEXT("/Script/Y.yCh_DataAsset'/Game/ArenaBattle/DataCharacterControl/Dty_Quater.Dty_Quater'"));
    if (QuaterRef.Object)
    {
        CharacterControlTMap.Add(ECharacterControlDataType::Quater, QuaterRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/Am_ComboAttack.Am_ComboAttack'"));
    if (ComboActionMontageRef.Object)
    {
        ComboActionMontage = ComboActionMontageRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UyComboDataAsset> ComboActionDataRef(TEXT("/Script/Y.yComboDataAsset'/Game/ArenaBattle/CharacterAsset/Da_ComboAttack.Da_ComboAttack'"));
    if (ComboActionDataRef.Object)
    {
        ComboActionData = ComboActionDataRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/Am_Dead.Am_Dead'"));
    if (DeadMontageRef.Object)
    {
        DeadMontage = DeadMontageRef.Object;
    }

	//Stat Component

	Stat = CreateDefaultSubobject<UyCharacterStatComponent>(TEXT("Stat"));
	
	//Widget Component
	HpBar = CreateDefaultSubobject<UABWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
    static ConstructorHelpers::FClassFinder<UABUserWidget>HpBarWidgetRef(TEXT("/Game/ArenaBattle/UI/Wbp_HpBar.Wbp_HpBar_C"));
    if (HpBarWidgetRef.Class)
    {
        HpBar->SetWidgetClass(HpBarWidgetRef.Class);
        HpBar->SetWidgetSpace(EWidgetSpace::Screen);
        HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
        HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    }

    // Item Actions
	
	/*TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject
	(this, &AyCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject 
	(this, &AyCharacterBase::ReadScroll)));*/

    FOnTakeItemDelegate EquipWeaponDelegate;
    EquipWeaponDelegate.BindUObject(this, &AyCharacterBase::EquipWeapon);
    TakeItemActions.Add(FTakeItemDelegateWrapper(EquipWeaponDelegate));

    FOnTakeItemDelegate DrinkPotionDelegate;
    DrinkPotionDelegate.BindUObject(this, &AyCharacterBase::DrinkPotion);
    TakeItemActions.Add(FTakeItemDelegateWrapper(DrinkPotionDelegate));

    FOnTakeItemDelegate ReadScrollDelegate;
    ReadScrollDelegate.BindUObject(this, &AyCharacterBase::ReadScroll);
    TakeItemActions.Add(FTakeItemDelegateWrapper(ReadScrollDelegate));

	//Weapon Component
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));


}

void AyCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	Stat->OnHpZero.AddUObject(this, &AyCharacterBase::SetDead);
    Stat->OnStatChanged.AddUObject(this, &AyCharacterBase::ApplyStat);
}

void AyCharacterBase::SetCharacterControlData(const UyCh_DataAsset* InyCh_DataAsset)
{
    //Pawn
    bUseControllerRotationYaw = InyCh_DataAsset->bUseControllerRotationYaw;

    //CharacterMovement
    GetCharacterMovement()->bOrientRotationToMovement = InyCh_DataAsset->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = InyCh_DataAsset->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = InyCh_DataAsset->RotationRate;

}

void AyCharacterBase::ProcessComboCommand()
{
    if (CurrentCombo == 0)
    {
        ComboActionBegin();
        return;
    }

    if (!ComboTimerHandle.IsValid())
    {
        HasNextComboCommand = false;

    }
    else
    {
        HasNextComboCommand = true;
    }
}

void AyCharacterBase::ComboActionBegin()
{
    //ComboStatus
    CurrentCombo = 1;

    //Movement Setting
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    //AnimationSetting
    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AyCharacterBase::ComboActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();

}

void AyCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    ensure(CurrentCombo != 0);
    CurrentCombo = 0;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    NotifyComboActionEnd();

}

void AyCharacterBase::NotifyComboActionEnd()
{
}

void AyCharacterBase::SetComboCheckTimer()
{
    int32 ComboIndex = CurrentCombo - 1;
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;

    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex]) / ComboActionData->FrameRate / AttackSpeedRate;

    if (ComboEffectiveTime > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AyCharacterBase::ComboCheck, ComboEffectiveTime, false);
    }

	
}

void AyCharacterBase::ComboCheck()
{
    ComboTimerHandle.Invalidate();
    if (HasNextComboCommand)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

        CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
        AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
        SetComboCheckTimer();
        HasNextComboCommand = false;
    }
}

void AyCharacterBase::AttackHitCheck()
{
    FHitResult OutHitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

    const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
    const float AttackDamage = Stat->GetTotalStat().Attack;
    const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
    const FVector End = Start + GetActorForwardVector() * AttackRange;

    bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
    if (HitDetected)
    {
        FDamageEvent DamageEvent;
        OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
    }

#if ENABLE_DRAW_DEBUG

    FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
    float CapsuleHalfHeight = AttackRange * 0.5f;
    FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

    DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius,
        FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);


#endif
}

float AyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);
    
    return DamageAmount;
}

void AyCharacterBase::SetDead()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    PlayDeadAnimation();
    SetActorEnableCollision(false);
    HpBar->SetHiddenInGame(true);
}

void AyCharacterBase::PlayDeadAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->StopAllMontages(0.0f);
    AnimInstance->Montage_Play(DeadMontage, 1.0f);


}

void AyCharacterBase::SetupCharacterWidget(UABUserWidget* InUserWidget)
{
	UyHpBarWidget* HpBarWidget = Cast<UyHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		//HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
        HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
        HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UyHpBarWidget::UpdateHpBar);
        Stat->OnStatChanged.AddUObject(HpBarWidget, &UyHpBarWidget::UpdateStat);
	}
}

void AyCharacterBase::TakeItem(UABItemData* InItemData)
{
    if (InItemData)
    {
        TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
    }
}

void AyCharacterBase::DrinkPotion(UABItemData* InItemData)
{
    UABPotionItemData* PotionItemData = Cast<UABPotionItemData>(InItemData);
	if (PotionItemData)
	{
        Stat->HealHp(PotionItemData->HealAmount);
		
	}
}

void AyCharacterBase::EquipWeapon(UABItemData* InItemData)
{
	UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);

	if (WeaponItemData)
	{
        if (WeaponItemData->WeaponMesh.IsPending())
        {
            WeaponItemData->WeaponMesh.LoadSynchronous();

        }
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
        /*Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh);*/
        Stat->SetModifierStat(WeaponItemData->ModifierStat);
	}
    //Weapon Component
   /* Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
    Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));*/

	
}

void AyCharacterBase::ReadScroll(UABItemData* InItemData)
{
    UABScrollItemData* ScrollItemData = Cast<UABScrollItemData > (InItemData);
    if (ScrollItemData)
    {
        Stat->AddBaseStat(ScrollItemData->BaseStat);
    }
    //UE_LOG(LogCharacterBase, Log, TEXT("ReadScroll"));
}

int32 AyCharacterBase::GetLevel()
{
    return Stat->GetCurrentLevel();
}

void AyCharacterBase::SetLevel(int32 InNew)
{
    Stat->SetLevelStat(InNew);
}

void AyCharacterBase::ApplyStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
    float MovementSpeed = (BaseStat+ ModifierStat).MovementSpeed;
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}


