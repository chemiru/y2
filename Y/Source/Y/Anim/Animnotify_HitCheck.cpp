// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Animnotify_HitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"

void UAnimnotify_HitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}

}
