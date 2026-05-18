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

	virtual UAnimMontage* AttackToPlayer() override;
	virtual void AttackHitCheck() override;

	UAnimMontage* SkillAttackToPlayer();
	UAnimMontage* PhaseSkillAttackToPlayer();
	virtual UAnimMontage* DodgeBackward();

	FORCEINLINE int32 GetNormalAttackCount() const { return Count_NormalAttack; }
	FORCEINLINE bool IsPhase2() const { return bIsPhase2; }
	void ResetNormalAttackCount();

protected:
	virtual void BeginPlay() override;
	virtual void OnDead() override;
	virtual float GetCurrentAttackDamage() const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = "Phase")
	float Phase2HPThreshold;

	UPROPERTY(EditAnywhere, Category = "Dodge", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float DodgeChance = 0.3f;

private:
	int32 Count_NormalAttack;
	bool bIsPhase2;

public:
	virtual void Tick(float DeltaTime) override;
};
