#include "Enemy/Boss/BossEnemy.h"

#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/DataTable/DA_BaseEnemyAnim.h"


ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossEnemy::OnDead()
{
	Super::OnDead();
}

void ABossEnemy::AttackHitCheck()
{
	Super::AttackHitCheck();
}

void ABossEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
	
	//TODO:: 리팩토링 고민중
	int64 RandomNum = FMath::RandRange(1,10);
	UE_LOG(LogTemp,Warning,TEXT("RandomNum %lld"),RandomNum);
	if (RandomNum % 2 == 0)
	{
		AnimInst->Montage_Play(Enemy_CombatComp->GetAnimMontage(EAnimMontage::AM_SingleAttack));
	}
	else
	{
		AnimInst->Montage_Play(Enemy_CombatComp->GetAnimMontage(EAnimMontage::AM_3ComboAttack));
	}
}