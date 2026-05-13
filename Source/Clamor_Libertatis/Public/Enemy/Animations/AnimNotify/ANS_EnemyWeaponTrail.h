#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "ANS_EnemyWeaponTrail.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UANS_EnemyWeaponTrail : public UAnimNotifyState_TimedNiagaraEffect
{
	GENERATED_BODY()

public:
	UANS_EnemyWeaponTrail();

protected:
	virtual UFXSystemComponent* SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const override;
};
