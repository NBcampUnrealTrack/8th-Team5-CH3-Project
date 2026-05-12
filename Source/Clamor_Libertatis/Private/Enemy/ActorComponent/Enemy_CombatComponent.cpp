#include "Enemy/ActorComponent/Enemy_CombatComponent.h"



UEnemy_CombatComponent::UEnemy_CombatComponent()
{
	DA_EnemySkill = nullptr;
	PrimaryComponentTick.bCanEverTick = true;
}

float UEnemy_CombatComponent::GetAttackDistance(EAttackType AttackType, int32 AttackIndex) const
{
	float Distance = 0.f;
	
	if (DA_EnemySkill)
	{
		if (GetEnemySkillArray(AttackType).IsValidIndex(AttackIndex))
		{
			Distance = GetEnemySkillArray(AttackType)[AttackIndex].AttackCollision.Distance;
		}
	}
	
	return Distance;
}

const FEnemySkillInfo& UEnemy_CombatComponent::GetSkillInfo(EAttackType AttackType, int32 AttackIndex) const
{
	return GetEnemySkillArray(AttackType)[AttackIndex];
}

FCollisionShape UEnemy_CombatComponent::MakeAttackCollision(EAttackType AttackType, int32 AttackIndex)
{
	FCollisionShape CollisionShape;
	
	if (DA_EnemySkill)
	{
		if (GetEnemySkillArray(AttackType).IsValidIndex(AttackIndex))
		{
			FEnemySkillInfo SkillInfo = GetEnemySkillArray(AttackType)[AttackIndex];
			
			if (SkillInfo.AttackCollision.CollisionShape == EAttackCollisionShape::Box)
			{
				CollisionShape = FCollisionShape::MakeBox(SkillInfo.AttackCollision.CollisionSize);
			}
			else if (SkillInfo.AttackCollision.CollisionShape == EAttackCollisionShape::Sphere)
			{
				CollisionShape = FCollisionShape::MakeSphere(SkillInfo.AttackCollision.SphereRadius);
			}
		}
	}
	return CollisionShape;
}


void UEnemy_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

const TArray<FEnemySkillInfo>& UEnemy_CombatComponent::GetEnemySkillArray(EAttackType AttackType) const
{
	static const TArray<FEnemySkillInfo> Empty_SkillInfo;
	
	switch (AttackType)
	{
	case EAttackType::Attack_Normal:
		return DA_EnemySkill->Array_NormalAttack;
	case EAttackType::Attack_Skill:
		return DA_EnemySkill->Array_Skill_Attack;
	case EAttackType::Attack_Skill_Phase:
		return DA_EnemySkill->Array_Skill_PhaseAttack;
	case EAttackType::NONE:
		return Empty_SkillInfo;
	}
	
	return Empty_SkillInfo;
}


void UEnemy_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

