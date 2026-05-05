#include "Enemy/ActorComponent/Enemy_StatComponent.h"

#include "Enemy/DataTable/DT_BaseEnemy.h"


UEnemy_StatComponent::UEnemy_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemy_StatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemy_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UEnemy_StatComponent::InitializeStat()
{
	if (!DT_EnemyRowHandle.IsNull())
	{
		const static FString ContextString = "Initialized EnemyStat";
		if (FBaseEnemyStat* EnemyStats = DT_EnemyRowHandle.GetRow<FBaseEnemyStat>(ContextString))
		{
			EnemyStat.HP = EnemyStats->Enemy_HP;
			EnemyStat.Attack_Damage = EnemyStats->Enemy_AttackDamage;
			EnemyStat.SightRadius = EnemyStats->Enemy_SightRadius;
			EnemyStat.LoseSightRadius = EnemyStats->Enemy_LoseSightRadius;
			EnemyStat.PeripheralVisionAngleDegrees = EnemyStats->Enemy_PeripheralVisionAngleDegrees;
			EnemyStat.SightConfig_MaxAge = EnemyStats->Enemy_SightConfig_MaxAge;
			UE_LOG(LogTemp,Warning,TEXT("HP: %f, Attack Damage : %f"),EnemyStat.HP, EnemyStat.Attack_Damage);
		}
	}
	
}