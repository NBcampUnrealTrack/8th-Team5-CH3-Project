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
};