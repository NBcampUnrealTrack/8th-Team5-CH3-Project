// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_SkillData.generated.h"


UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Melee,
    Projectile,
    Area,
    Buff,
    Summon
};

UCLASS()
class CLAMOR_LIBERTATIS_API UDA_SkillData : public UDataAsset
{
	GENERATED_BODY()
public:
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SkillName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName Description;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ESkillType SkillType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float StaminaCost = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ManaCost = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Cooldown = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Damage = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float SkillRange = 0.f;
    
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SphereRadius = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> CastMontage; //시전동작

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AActor> ProjectileClass;
};
