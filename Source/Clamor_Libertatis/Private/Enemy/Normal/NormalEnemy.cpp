#include "Enemy/Normal/NormalEnemy.h"

#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"


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

void ANormalEnemy::OnDead()
{
	Super::OnDead();
}

void ANormalEnemy::AttackHitCheck()
{
	Super::AttackHitCheck();
}


void ANormalEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
}