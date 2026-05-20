// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Anim/AM_CharacterSuperarmor.h"
#include "Combat/CombatComponent.h"

void UAM_CharacterSuperarmor::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	if (UCombatComponent* CombatComponent = Owner->FindComponentByClass<UCombatComponent>())
	{
		CombatComponent->SetSuperarmor(true);
	}
}

void UAM_CharacterSuperarmor::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	if (UCombatComponent* CombatComponent = Owner->FindComponentByClass<UCombatComponent>())
	{
		CombatComponent->SetSuperarmor(false);
	}
}
