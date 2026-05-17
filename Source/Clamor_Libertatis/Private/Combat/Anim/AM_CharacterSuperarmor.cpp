// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Anim/AM_CharacterSuperarmor.h"
#include "Combat/CombatComponent.h"

void UAM_CharacterSuperarmor::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
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

void UAM_CharacterSuperarmor::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
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
