#pragma once

#include "CoreMinimal.h"
#include "Enemy/Boss/BossEnemy_Mage.h"
#include "NormalEnemy_Mage.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ANormalEnemy_Mage : public ABossEnemy_Mage
{
	GENERATED_BODY()

public:
	ANormalEnemy_Mage();

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void HitReaction();
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};