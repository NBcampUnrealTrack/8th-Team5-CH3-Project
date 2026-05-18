#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "DA_BaseEnemySkill.generated.h"

class ABaseThrowMagic;

UENUM()
enum class EAttackType : uint8
{
	NONE,
	Attack_Normal,
	Attack_Skill,
	Attack_Skill_Phase
};

UENUM()
enum class EAttackCollisionShape : uint8
{
	Box,
	Sphere
};

USTRUCT(BlueprintType)
struct FEnemyAttackCollision
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	EAttackCollisionShape CollisionShape;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	float Distance = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	FVector CollisionSize = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	float SphereRadius = 0.f;
};

USTRUCT(BlueprintType)
struct FEnemySkillInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackType")
	EAttackType AttackType = EAttackType::NONE;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animations|AnimMontage")
	TObjectPtr<UAnimMontage> AM_TypeMontage = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Collision|Attack")
	FEnemyAttackCollision AttackCollision;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float DamageMultiplier = 1.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="VFX")
	TObjectPtr<UNiagaraSystem> SkillVFX = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType")
	bool bIsLaunch = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType|Projectile",
		meta=(EditCondition="bIsLaunch", EditConditionHides))
	TSubclassOf<ABaseThrowMagic> ProjectileClass = nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType|Projectile",
		meta=(EditCondition="bIsLaunch", EditConditionHides, ClampMin=1))
	int32 ProjectileCount = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType|Projectile",
		meta=(EditCondition="bIsLaunch", EditConditionHides, ClampMin=0.f, ClampMax=360.f))
	float SpreadAngle = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType|Projectile")
	bool bIsHoming = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType")
	bool bIsLaserAttack = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType|Laser",
		meta=(EditCondition="bIsLaserAttack", EditConditionHides, ClampMin=100.f))
	float LaserRange = 3000.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType|Laser",
		meta=(EditCondition="bIsLaserAttack", EditConditionHides, ClampMin=1.f))
	float LaserRadius = 30.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SkillType|Laser",
		meta=(EditCondition="bIsLaserAttack", EditConditionHides))
	TObjectPtr<UNiagaraSystem> LaserVFX = nullptr;
};


UCLASS()
class CLAMOR_LIBERTATIS_API UDA_BaseEnemySkill : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="EnemySkillInfo")
	TArray<FEnemySkillInfo> Array_NormalAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="EnemySkillInfo")
	TArray<FEnemySkillInfo> Array_Skill_Attack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="EnemySkillInfo")
	TArray<FEnemySkillInfo> Array_Skill_PhaseAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage|Dead")
	TObjectPtr<UAnimMontage> AM_Dead;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage|HitReaction")
	TObjectPtr<UAnimMontage> AM_HitReaction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage|Dodge")
	TObjectPtr<UAnimMontage> AM_Dodge;
};
