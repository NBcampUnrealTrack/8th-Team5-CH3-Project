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
	
	virtual void AttackToPlayer() override;
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};
