#include "Enemy/Boss/BossEnemy.h"

#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"


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
}