// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponSocketItemData.generated.h"

class UDA_SkillData;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EWeaponSocketElement : uint8
{
	None UMETA(DisplayName = "None"),
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice"),
	Lightning UMETA(DisplayName = "Lightning"),
	Poison UMETA(DisplayName = "Poison"),
	Holy UMETA(DisplayName = "Holy"),
	Dark UMETA(DisplayName = "Dark")
};

UENUM(BlueprintType)
enum class EWeaponSocketType : uint8
{
	None UMETA(DisplayName = "None"),
	Blade UMETA(DisplayName = "Blade"),
	Grip UMETA(DisplayName = "Grip"),
	All UMETA(DisplayName = "All"),
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FWeaponSocketItemModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Modifier")
	float AdditiveDamage = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Modifier", meta = (ClampMin = "0.0"))
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Modifier", meta = (ClampMin = "0.0"))
	float StaminaCostMultiplier = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Modifier", meta = (ClampMin = "0.0"))
	float ManaCostMultiplier = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Modifier", meta = (ClampMin = "0.0"))
	float AttackSpeedMultiplier = 1.0f;
};

UCLASS(BlueprintType)
class CLAMOR_LIBERTATIS_API UWeaponSocketItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(BlueprintPure, Category = "SocketItem")
	bool IsCompatibleWithSocket(EWeaponSocketType SocketTag) const;

	UFUNCTION(BlueprintPure, Category = "SocketItem")
	bool IsCompatibleWithWeaponTags(const TArray<EWeaponSocketType>& WeaponTags) const;

	UFUNCTION(BlueprintPure, Category = "SocketItem")
	float GetModifiedDamage(float BaseDamage) const;

	UFUNCTION(BlueprintPure, Category = "SocketItem")
	float GetModifiedStaminaCost(float BaseStaminaCost) const;

	UFUNCTION(BlueprintPure, Category = "SocketItem")
	float GetModifiedManaCost(float BaseManaCost) const;

	UFUNCTION(BlueprintPure, Category = "SocketItem")
	float GetModifiedAttackSpeed(float BaseAttackSpeed) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem")
	FName SocketItemId = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem")
	FName InventoryItemID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem")
	EWeaponSocketElement Element = EWeaponSocketElement::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Compatibility")
	EWeaponSocketType CompatibleSocketTag = EWeaponSocketType::All;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Compatibility")
	TArray<EWeaponSocketType> CompatibleWeaponTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Modifier")
	FWeaponSocketItemModifier Modifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|VFX")
	TObjectPtr<UNiagaraSystem> TrailNiagaraOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|VFX")
	TObjectPtr<UNiagaraSystem> HitNiagaraOverride;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SocketItem|Skill")
	TObjectPtr<UDA_SkillData> SkillDataOverride;
};
