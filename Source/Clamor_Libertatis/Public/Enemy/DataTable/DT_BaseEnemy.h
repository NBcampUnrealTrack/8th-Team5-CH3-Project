#pragma once

#include "CoreMinimal.h"
#include "DT_BaseEnemy.generated.h"

USTRUCT(BlueprintType)
struct FBaseEnemyStat : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_HP;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_AttackDamage;
};
