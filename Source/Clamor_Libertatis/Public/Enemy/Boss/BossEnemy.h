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

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
