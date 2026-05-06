#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInst.generated.h"

class ABaseEnemy;

UCLASS()
class CLAMOR_LIBERTATIS_API UBaseEnemyAnimInst : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stat")
	float Enemy_GroundSpeed;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Instance")
	TObjectPtr<ABaseEnemy> MyOwnerInst;
};
