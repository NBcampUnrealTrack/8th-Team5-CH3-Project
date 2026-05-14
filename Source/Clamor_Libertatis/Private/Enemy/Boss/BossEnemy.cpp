#include "Enemy/Boss/BossEnemy.h"
#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"

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

UAnimMontage* ABossEnemy::AttackToPlayer()
{
	if (UAnimMontage* Montage = Super::AttackToPlayer())
	{
		Count_NormalAttack++;
		return Montage;
	}
	return nullptr;
}

void ABossEnemy::AttackHitCheck()
{
	Super::AttackHitCheck();
}

UAnimMontage* ABossEnemy::SkillAttackToPlayer()
{
	int32 SkillCount = Enemy_CombatComp->GetSkillCount(EAttackType::Attack_Skill);
	if (SkillCount <= 0) return nullptr;

	int32 RandomNum = FMath::RandRange(0, SkillCount - 1);
	UAnimMontage* Montage = Enemy_CombatComp->GetAttackMontage(EAttackType::Attack_Skill, RandomNum);
	AnimInst->Montage_Play(Montage);
	CurrentAttackData.Key = EAttackType::Attack_Skill;
	CurrentAttackData.Value = RandomNum;
	ResetNormalAttackCount();
	return Montage;
}

void ABossEnemy::ResetNormalAttackCount()
{
	Count_NormalAttack = 0;
}

float ABossEnemy::GetCurrentAttackDamage() const
{
	float BaseDamage = Enemy_StatComp->GetEnemyStat().Attack_Damage;
	if (CurrentAttackData.Key == EAttackType::Attack_Skill || CurrentAttackData.Key == EAttackType::Attack_Skill_Phase)
	{
		const FEnemySkillInfo& SkillInfo = Enemy_CombatComp->GetSkillInfo(CurrentAttackData.Key, CurrentAttackData.Value);
		BaseDamage *= SkillInfo.DamageMultiplier;
	}
	return BaseDamage;
}