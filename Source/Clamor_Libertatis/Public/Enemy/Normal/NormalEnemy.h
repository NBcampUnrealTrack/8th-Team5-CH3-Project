#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "NormalEnemy.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ANormalEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ANormalEnemy();
	
	virtual UAnimMontage* AttackToPlayer() override;
	virtual void AttackHitCheck() override;
protected:
	virtual void BeginPlay() override;
	virtual void OnDead() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void HitReaction();
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	virtual void Tick(float DeltaTime) override;
};
