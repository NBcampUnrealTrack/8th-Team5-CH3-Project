// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BaseCombatAnimInstance.h"
#include "Combat/CombatComponent.h"

void UBaseCombatAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwningPawn = TryGetPawnOwner();

    if (OwningPawn)
    {
        CombatComponent = OwningPawn->FindComponentByClass<UCombatComponent>();
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
