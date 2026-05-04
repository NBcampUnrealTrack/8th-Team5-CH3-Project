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

void UEnemy_StatComponent::InitializeEnemyStat()
{
	if (!DT_EnemyRowHandle.IsNull())
	{
		const static FString ContextString = "Initialized Stat by Row";
		if (FBaseEnemyStat* EnemyStat = DT_EnemyRowHandle.GetRow<FBaseEnemyStat>(ContextString))
		{
			HP = EnemyStat->Enemy_HP;
			Attack_Damage = EnemyStat->Enemy_AttackDamage;
			UE_LOG(LogTemp,Warning,TEXT("Initialized Enemy Stat"));
		}
	}
}