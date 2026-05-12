#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "BossEnemy.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ABossEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ABossEnemy();

	virtual void AttackToPlayer() override;
	virtual void AttackHitCheck() override;

	void SkillAttackToPlayer();

	FORCEINLINE int32 GetNormalAttackCount() const { return Count_NormalAttack; }
	void ResetNormalAttackCount();

protected:
	virtual void BeginPlay() override;
	virtual void OnDead() override;
	virtual float GetCurrentAttackDamage() const override;

private:
	int32 Count_NormalAttack;
public:
	virtual void Tick(float DeltaTime) override;
};
