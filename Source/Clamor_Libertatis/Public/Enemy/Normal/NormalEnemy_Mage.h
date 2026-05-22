#pragma once

#include "CoreMinimal.h"
#include "Enemy/Boss/BossEnemy_Mage.h"
#include "Enemy/Interface/IEnemyHitReactable.h"
#include "NormalEnemy_Mage.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ANormalEnemy_Mage : public ABossEnemy_Mage, public IEnemyHitReactable
{
	GENERATED_BODY()

public:
	ANormalEnemy_Mage();

	virtual void HitReaction() override;
	virtual bool IsInHitReaction() const override { return bIsInHitReaction; }

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	bool bIsInHitReaction = false;
};