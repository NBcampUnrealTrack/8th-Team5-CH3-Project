#pragma once

#include "CoreMinimal.h"
#include "BossEnemy.h"
#include "Enemy/Interface/IMageCaster.h"
#include "BossEnemy_Mage.generated.h"

class ABaseThrowMagic;

UCLASS()
class CLAMOR_LIBERTATIS_API ABossEnemy_Mage : public ABossEnemy , public IMageCaster
{
	GENERATED_BODY()

public:
	ABossEnemy_Mage();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void AttackHitCheck() override;
	virtual void SpawnProjectile(const FEnemySkillInfo& SkillInfo) override;
	virtual FName GetProjectileSpawnSocket() const override;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float HomingAccelerationMagnitude = 1500.f;

	void SpawnChargeProjectile(FName SocketName);
	void LaunchChargeProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ABaseThrowMagic> PendingProjectile;

};
