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
	
	void PlayAM_Attack_Sweep();
	void PlayAM_JumpAttack();
	void PlayAM_SingleAttack();
	void PlayAM_3ComboAttack();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stat")
	float Enemy_GroundSpeed;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Instance")
	TObjectPtr<ABaseEnemy> MyOwnerInst;
#pragma region AnimMontage
	//Normal TempLogic
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage")
	TObjectPtr<UAnimMontage> AM_Attack_Sweep;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage")
	TObjectPtr<UAnimMontage> AM_JumpAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage")
	TObjectPtr<UAnimMontage> AM_SingleAttack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage")
	TObjectPtr<UAnimMontage> AM_3ComboAttack;
#pragma endregion 
};
