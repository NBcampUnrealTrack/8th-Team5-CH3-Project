#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Interface/IEnemyHitReactable.h"
#include "NormalEnemy.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ANormalEnemy : public ABaseEnemy, public IEnemyHitReactable
{
	GENERATED_BODY()

public:
	ANormalEnemy();

	virtual UAnimMontage* AttackToPlayer() override;
	virtual void AttackHitCheck() override;

	virtual void HitReaction() override;
	virtual bool IsInHitReaction() const override { return bIsInHitReaction; }

protected:
	virtual void BeginPlay() override;
	virtual void OnDead() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsInHitReaction = false;
};
