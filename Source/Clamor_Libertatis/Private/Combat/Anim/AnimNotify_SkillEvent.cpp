// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Anim/AnimNotify_SkillEvent.h"

#include "Combat/SkillComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

void UAnimNotify_SkillEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
	{
		SkillComponent->ExecuteSkillEvent(EventID);
	}
}
