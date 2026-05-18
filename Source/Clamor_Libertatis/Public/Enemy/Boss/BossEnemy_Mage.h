#pragma once

#include "CoreMinimal.h"
#include "BossEnemy.h"
#include "Enemy/Interface/IMageCaster.h"
#include "BossEnemy_Mage.generated.h"

class ABaseThrowMagic;
class ABaseEnemy;
class UBoxComponent;

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
	void FireLaser(const FEnemySkillInfo& SkillInfo);
	void SummonMinions();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<TObjectPtr<ABaseThrowMagic>> PendingProjectiles;

	UPROPERTY(VisibleAnywhere, Category="Summon")
	TObjectPtr<UBoxComponent> SummonSpawnBox;

	UPROPERTY(EditDefaultsOnly, Category="Summon")
	TArray<TSubclassOf<ABaseEnemy>> SummonMinionClasses;

};
