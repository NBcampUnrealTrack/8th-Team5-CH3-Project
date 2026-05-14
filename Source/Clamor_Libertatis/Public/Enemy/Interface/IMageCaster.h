#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "IMageCaster.generated.h"

UINTERFACE(MinimalAPI)
class UMageCaster : public UInterface
{
	GENERATED_BODY()
};

class CLAMOR_LIBERTATIS_API IMageCaster
{
	GENERATED_BODY()

public:
	// 투사체 스폰 (일반 공격 / 스킬 공격 구분)
	virtual void SpawnProjectile(const FEnemySkillInfo& SkillInfo) = 0;

	// 투사체 발사 위치 소켓 이름 (예: "MuzzleSocket", "HandSocket")
	virtual FName GetProjectileSpawnSocket() const = 0;
};