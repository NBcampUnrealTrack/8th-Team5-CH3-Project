#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_BaseEnemySkill.generated.h"

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
};
