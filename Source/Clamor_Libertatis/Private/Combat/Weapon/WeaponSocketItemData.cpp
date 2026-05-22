// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat/Weapon/WeaponSocketItemData.h"

FPrimaryAssetId UWeaponSocketItemData::GetPrimaryAssetId() const
{
	const FName AssetName = SocketItemId.IsNone() ? GetFName() : SocketItemId;
	return FPrimaryAssetId(TEXT("WeaponSocketItem"), AssetName);
}

bool UWeaponSocketItemData::IsCompatibleWithSocket(EWeaponSocketType SocketTag) const
{
	return CompatibleSocketTag == EWeaponSocketType::All || CompatibleSocketTag == SocketTag;
}

bool UWeaponSocketItemData::IsCompatibleWithWeaponTags(const TArray<EWeaponSocketType>& WeaponTags) const
{
	if (CompatibleWeaponTags.IsEmpty())
	{
		return true;
	}

	for (const EWeaponSocketType& WeaponTag : WeaponTags)
	{
		if (CompatibleWeaponTags.Contains(WeaponTag))
		{
			return true;
		}
	}

	return false;
}

float UWeaponSocketItemData::GetModifiedDamage(float BaseDamage) const
{
	return (BaseDamage + Modifier.AdditiveDamage) * Modifier.DamageMultiplier;
}

float UWeaponSocketItemData::GetModifiedStaminaCost(float BaseStaminaCost) const
{
	return BaseStaminaCost * Modifier.StaminaCostMultiplier;
}

float UWeaponSocketItemData::GetModifiedManaCost(float BaseManaCost) const
{
	return BaseManaCost * Modifier.ManaCostMultiplier;
}

float UWeaponSocketItemData::GetModifiedAttackSpeed(float BaseAttackSpeed) const
{
	return BaseAttackSpeed * Modifier.AttackSpeedMultiplier;
}
