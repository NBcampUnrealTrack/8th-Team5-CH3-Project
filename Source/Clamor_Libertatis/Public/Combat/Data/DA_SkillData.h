// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_SkillData.generated.h"

class AActor;
class UAnimMontage;

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
class CLAMOR_LIBERTATIS_API UDA_SkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("SkillData"), GetFName());
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FName SkillName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	ESkillType SkillType = ESkillType::Projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Cost")
	float StaminaCost = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Cost")
	float ManaCost = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Cooldown")
	float Cooldown = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Effect")
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Effect")
	float SkillRange = 10000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Effect")
	float SphereRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation")
	TObjectPtr<UAnimMontage> CastMontage; //시전동작

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Spawn")
	TSubclassOf<AActor> ProjectileClass;


};
