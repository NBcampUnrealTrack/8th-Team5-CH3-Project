#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Enemy_AIController.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_AIController();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};
