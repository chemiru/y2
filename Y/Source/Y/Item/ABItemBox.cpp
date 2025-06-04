// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Item/ABItemData.h"
#include "Engine/AssetManager.h"
#include "ABItemData.h"


// Sets default values
AABItemBox::AABItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	/*Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);*/

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAssetRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (MeshAssetRef.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAssetRef.Object);
	}

	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectAssetRef(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectAssetRef.Succeeded())
	{
		Effect->SetTemplate(EffectAssetRef.Object);
		Effect->bAutoActivate = false;
	}
}

void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	ensure(0 < Assets.Num());

	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}

	Item = Cast<UABItemData>(AssetPtr.Get());
	ensure(Item);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
}

void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Item == nullptr)
	{
		Destroy();
		return;
	}
	
	
	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}
	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

