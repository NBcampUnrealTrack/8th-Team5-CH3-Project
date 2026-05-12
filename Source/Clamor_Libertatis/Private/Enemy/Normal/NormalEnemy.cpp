#include "Enemy/Normal/NormalEnemy.h"

#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
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

void ANormalEnemy::OnDead()
{
	Super::OnDead();
}

float ANormalEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HitReaction();
	
	return ActualDamage;
}

void ANormalEnemy::HitReaction()
{
	UE_LOG(LogTemp,Warning,TEXT("HitReaction"));
	
	if (AnimInst)
	{
		if (UAnimMontage* AM_Hit = Enemy_CombatComp->DA_EnemySkill->AM_HitReaction)
		{
			AnimInst->Montage_Play(AM_Hit);
		}
	}
}

void ANormalEnemy::AttackHitCheck()
{
	Super::AttackHitCheck();
}


void ANormalEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
}