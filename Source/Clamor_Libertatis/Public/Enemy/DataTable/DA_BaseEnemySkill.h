#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_BaseEnemyAnim.generated.h"

UENUM()
enum class EAnimMontage : uint8
{
	AM_SweepAttack,
	AM_JumpAttack,
	AM_SingleAttack,
	AM_3ComboAttack
};




UCLASS()
class CLAMOR_LIBERTATIS_API UDA_BaseEnemyAnim : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage|Attack")
	TMap<EAnimMontage,TObjectPtr<UAnimMontage>> Map_AM_Battle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AnimMontage|Dead")
	TObjectPtr<UAnimMontage> AM_Dead;
};
