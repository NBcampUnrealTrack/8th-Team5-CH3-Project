#include "Enemy/Normal/NormalEnemy.h"

#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/DataTable/DA_BaseEnemyAnim.h"


ANormalEnemy::ANormalEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANormalEnemy::OnDead()
{
	Super::OnDead();
}


void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANormalEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
	
	
	int64 RandomNum = FMath::RandRange(1,10);
	UE_LOG(LogTemp,Warning,TEXT("RandomNum %lld"),RandomNum);
	if (RandomNum % 2 == 0)
	{
		AnimInst->Montage_Play(Enemy_CombatComp->GetAnimMontage(EAnimMontage::AM_SweepAttack));
	}
	else
	{
		AnimInst->Montage_Play(Enemy_CombatComp->GetAnimMontage(EAnimMontage::AM_SweepAttack));
	}
	
	
}