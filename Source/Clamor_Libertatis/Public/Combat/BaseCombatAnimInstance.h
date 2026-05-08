// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCombatAnimInstance.generated.h"

class UCombatComponent;

UCLASS()
class CLAMOR_LIBERTATIS_API UBaseCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
    virtual void NativeInitializeAnimation() override;

protected:
    UCombatComponent* GetCombatComponent();

protected:
    UPROPERTY()
    APawn* OwningPawn;

    UPROPERTY()
    UCombatComponent* CombatComponent;

public:

#pragma region Attack
    UFUNCTION()
    void AnimNotify_EnableCombo();

    UFUNCTION()
    void AnimNotify_DisableCombo();
    
    UFUNCTION()
    void AnimNotify_CheckCombo();

    UFUNCTION()
    void AnimNotify_EndAttack();

    UFUNCTION()
    void AnimNotify_EnableWeaponHitbox();

    UFUNCTION()
    void AnimNotify_DisableWeaponHitbox();
#pragma endregion Attack

    UFUNCTION()
    void AnimNotify_UseSkill();

    
#pragma region Dodge
    UFUNCTION()
    void AnimNotify_EnableInvincible();
    
    UFUNCTION()
    void AnimNotify_DisableInvincible();

    UFUNCTION()
    void AnimNotify_DodgeEnd();
#pragma endregion Dodge

#pragma region HitReact
    
    UFUNCTION()
    void AnimNotify_StartHitReact();
    
    UFUNCTION()
    void AnimNotify_EndHitReact();


#pragma endregion GHitReactuard

#pragma region Guard



#pragma endregion Guard

};