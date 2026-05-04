#pragma once

#include "CoreMinimal.h"
#include "DT_BaseEnemy.generated.h"

USTRUCT(BlueprintType)
struct FBaseEnemyStat : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_HP = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_AttackDamage = 0.f;
};
