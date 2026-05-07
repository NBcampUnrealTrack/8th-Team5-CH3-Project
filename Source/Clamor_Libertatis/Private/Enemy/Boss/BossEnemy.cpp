#include "Enemy/Boss/BossEnemy.h"

#include "Enemy/Animations/BaseEnemyAnimInst.h"


ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossEnemy::OnDead()
{
	Super::OnDead();
}


void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
	
	//TODO:: 리팩토링 고민중
	int32 RandomNum = FMath::RandRange(0,10);
	if (AnimInst)
	{
		if (RandomNum % 2 == 0)
		{
			AnimInst->PlayAM_SingleAttack();
		}
		else
		{
			AnimInst->PlayAM_3ComboAttack();
		}
	}
}