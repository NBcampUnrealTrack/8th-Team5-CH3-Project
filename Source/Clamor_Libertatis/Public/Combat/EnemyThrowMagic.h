#pragma once

#include "CoreMinimal.h"
#include "Combat/BaseThrowMagic.h"
#include "EnemyThrowMagic.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API AEnemyThrowMagic : public ABaseThrowMagic
{
	GENERATED_BODY()

protected:
	virtual bool ShouldReactToHit(AActor* OtherActor) const override;
};
