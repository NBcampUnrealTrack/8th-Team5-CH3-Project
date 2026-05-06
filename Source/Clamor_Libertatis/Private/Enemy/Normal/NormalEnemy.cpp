#include "Enemy/Normal/NormalEnemy.h"

#include "Enemy/Animations/BaseEnemyAnimInst.h"


ANormalEnemy::ANormalEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}


void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANormalEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
	
	// For Test Random Play AM
	int32 RandomNum = FMath::RandRange(0,10);
	if (AnimInst)
	{
		if (RandomNum % 2 == 0)
		{
			AnimInst->PlayAM_Attack_Sweep();
		}
		else
		{
			AnimInst->PlayAM_JumpAttack();
		}
	}
}