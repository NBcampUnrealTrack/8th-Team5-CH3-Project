#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "Enemy_CombatComponent.generated.h"


class UDA_EnemyAttackCollision;
class UDA_BaseEnemySkill;
struct FBaseEnemyStat;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_CombatComponent();
	
	FORCEINLINE int32 GetSkillCount(EAttackType AttackType) const;
	FORCEINLINE float GetAttackDistance(EAttackType AttackType, int32 AttackIndex) const;
	FORCEINLINE UAnimMontage* GetAttackMontage(EAttackType AttackType, int32 AttackIndex) const {return GetEnemySkillArray(AttackType)[AttackIndex].AM_TypeMontage;}

	const FEnemySkillInfo& GetSkillInfo(EAttackType AttackType, int32 AttackIndex) const;
	FCollisionShape MakeAttackCollision(EAttackType AttackType,int32 AttackIndex);
	
	UPROPERTY()
	TObjectPtr<UDA_BaseEnemySkill> DA_EnemySkill;

protected:
	virtual void BeginPlay() override;
	
private:
	const TArray<FEnemySkillInfo>& GetEnemySkillArray(EAttackType AttackType) const;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
