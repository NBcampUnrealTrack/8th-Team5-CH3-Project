#pragma once

#include "CoreMinimal.h"
#include "BossEnemy.h"
#include "BossEnemy_Mage.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ABossEnemy_Mage : public ABossEnemy
{
	GENERATED_BODY()

public:
	
	ABossEnemy_Mage();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
