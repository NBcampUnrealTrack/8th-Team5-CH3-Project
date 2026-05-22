#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnemyHitReactable.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyHitReactable : public UInterface
{
	GENERATED_BODY()
};

class CLAMOR_LIBERTATIS_API IEnemyHitReactable
{
	GENERATED_BODY()

public:
	virtual void HitReaction() = 0;
	virtual bool IsInHitReaction() const = 0;
};