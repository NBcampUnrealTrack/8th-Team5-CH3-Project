#include "Enemy/Boss/BossEnemy.h"
#include "Enemy/ActorComponent/Enemy_ItemDropTableComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/AIController/Enemy_AIController.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	Count_NormalAttack = 0;
	bIsPhase2 = false;
	Phase2HPThreshold = 0.5f;

	ItemDropComp = CreateDefaultSubobject<UEnemy_ItemDropTableComponent>(TEXT("ItemDropTableComponent"));
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
	if (ItemDropComp)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		ItemDropComp->DropItem(SpawnLocation);
	}

	Super::OnDead();
}

float ABossEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsPhase2 && !bIsDead && Enemy_StatComp)
	{
		const FEnemyStat& Stat = Enemy_StatComp->GetEnemyStat();
		if (Stat.MaxHP > 0.f && (Stat.HP / Stat.MaxHP) <= Phase2HPThreshold)
		{
			bIsPhase2 = true;

			if (AEnemy_AIController* AIC = Cast<AEnemy_AIController>(GetController()))
			{
				if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
				{
					BB->SetValueAsBool(TEXT("bIsPhase2"), true);
				}
			}
		}
	}

	if (!bIsDead && ActualDamage > 0.f)
	{
		if (AEnemy_AIController* AIC = Cast<AEnemy_AIController>(GetController()))
		{
			if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
			{
				if (!BB->GetValueAsBool(TEXT("bShouldDodge")) && FMath::FRand() < DodgeChance)
				{
					BB->SetValueAsBool(TEXT("bShouldDodge"), true);
				}
			}
		}
	}

	return ActualDamage;
}

UAnimMontage* ABossEnemy::DodgeBackward()
{
	if (!Enemy_CombatComp || !Enemy_CombatComp->DA_EnemySkill) return nullptr;

	UAnimMontage* DodgeMontage = Enemy_CombatComp->DA_EnemySkill->AM_Dodge;
	if (!DodgeMontage || !AnimInst) return nullptr;

	AnimInst->Montage_Play(DodgeMontage);
	return DodgeMontage;
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
	BeginMontageAttack(Montage);
	return Montage;
}

UAnimMontage* ABossEnemy::PhaseSkillAttackToPlayer()
{
	int32 SkillCount = Enemy_CombatComp->GetSkillCount(EAttackType::Attack_Skill_Phase);
	if (SkillCount <= 0) return nullptr;

	int32 RandomNum = FMath::RandRange(0, SkillCount - 1);
	UAnimMontage* Montage = Enemy_CombatComp->GetAttackMontage(EAttackType::Attack_Skill_Phase, RandomNum);
	AnimInst->Montage_Play(Montage);
	CurrentAttackData.Key = EAttackType::Attack_Skill_Phase;
	CurrentAttackData.Value = RandomNum;
	ResetNormalAttackCount();
	BeginMontageAttack(Montage);
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