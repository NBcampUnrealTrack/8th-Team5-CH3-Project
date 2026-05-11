// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BaseCombatAnimInstance.h"
#include "Combat/CombatComponent.h"
#include "Combat/SkillComponent.h"

void UBaseCombatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = TryGetPawnOwner();

	if (OwningPawn)
	{
		CombatComponent = OwningPawn->FindComponentByClass<UCombatComponent>();
		SkillComponent = OwningPawn->FindComponentByClass<USkillComponent>();
	}
}

UCombatComponent* UBaseCombatAnimInstance::GetCombatComponent()
{
	if (!CombatComponent)
	{
		OwningPawn = TryGetPawnOwner();

		if (OwningPawn)
		{
			CombatComponent = OwningPawn->FindComponentByClass<UCombatComponent>();
		}
	}

	return CombatComponent;
}

USkillComponent* UBaseCombatAnimInstance::GetSkillComponent()
{
	if (!SkillComponent)
	{
		OwningPawn = TryGetPawnOwner();

		if (OwningPawn)
		{
			SkillComponent = OwningPawn->FindComponentByClass<USkillComponent>();
		}
	}

	return SkillComponent;
}

void UBaseCombatAnimInstance::AnimNotify_EnableCombo()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->EnableCombo();
	}
}

void UBaseCombatAnimInstance::AnimNotify_DisableCombo()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->DisableCombo();
	}
}

void UBaseCombatAnimInstance::AnimNotify_CheckCombo()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->CheckCombo();
	}
}

void UBaseCombatAnimInstance::AnimNotify_EndAttack()
{
	//체크용
	if (UCombatComponent* Comp = GetCombatComponent()) {
		//Comp->EndAttack();
	}
}

void UBaseCombatAnimInstance::AnimNotify_EnableWeaponHitbox()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->EnableWeaponHitbox();
	}
}

void UBaseCombatAnimInstance::AnimNotify_DisableWeaponHitbox()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->DisableWeaponHitbox();
	}
}

void UBaseCombatAnimInstance::AnimNotify_UseSkill()
{
	if (USkillComponent* Comp = GetSkillComponent()) {
		Comp->ExecutePendingSkill();
	}
}




void UBaseCombatAnimInstance::AnimNotify_EnableInvincible()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->SetInvincible(true);
	}
}

void UBaseCombatAnimInstance::AnimNotify_DisableInvincible()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->SetInvincible(false);
	}
}

void UBaseCombatAnimInstance::AnimNotify_DodgeEnd()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->EndDodge();
	}
}

void UBaseCombatAnimInstance::AnimNotify_StartHitReact()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->HitReact(true);
	}
}

void UBaseCombatAnimInstance::AnimNotify_EndHitReact()
{
	if (UCombatComponent* Comp = GetCombatComponent()) {
		Comp->HitReact(false);
	}
}
