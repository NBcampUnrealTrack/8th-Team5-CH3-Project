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
protected:
	virtual void BeginPlay() override;
	virtual void OnDead() override;
public:
	virtual void Tick(float DeltaTime) override;
};
