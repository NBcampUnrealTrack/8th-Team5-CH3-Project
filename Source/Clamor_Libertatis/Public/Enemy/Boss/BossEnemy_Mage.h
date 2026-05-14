#pragma once

#include "CoreMinimal.h"
#include "BossEnemy.h"
#include "Enemy/Interface/IMageCaster.h"
#include "BossEnemy_Mage.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ABossEnemy_Mage : public ABossEnemy , public IMageCaster
{
	GENERATED_BODY()

public:
	ABossEnemy_Mage();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SpawnProjectile(const FEnemySkillInfo& SkillInfo) override;
	virtual FName GetProjectileSpawnSocket() const override;
	
protected:
	virtual void BeginPlay() override;
	
};
