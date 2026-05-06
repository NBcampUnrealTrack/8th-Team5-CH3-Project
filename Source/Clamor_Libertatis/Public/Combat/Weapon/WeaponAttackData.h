// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAttackData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponAttackData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    FName SectionName = NAME_None;//Attack_Number

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    float Damage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
    float StaminaCost = 10.0f;
};
