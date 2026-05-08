#pragma once

#include "CoreMinimal.h"
#include "DA_BaseEnemyAnim.h"
#include "DT_BaseEnemy.generated.h"

USTRUCT(BlueprintType)
struct FBaseEnemyStat : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_HP = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_AttackDamage = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_SightRadius = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_LoseSightRadius = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_PeripheralVisionAngleDegrees = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="BaseEnemyStat")
	float Enemy_SightConfig_MaxAge = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataAsset")
	TObjectPtr<UDA_BaseEnemyAnim> DA_EnemyAnim = nullptr;
};
