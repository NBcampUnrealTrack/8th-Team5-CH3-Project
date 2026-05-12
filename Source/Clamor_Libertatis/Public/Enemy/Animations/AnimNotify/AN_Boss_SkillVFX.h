#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Boss_SkillVFX.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UAN_Boss_SkillVFX : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// VFX를 스폰할 소켓 이름 (에디터에서 스킬별로 지정)
	UPROPERTY(EditAnywhere, Category="VFX")
	FName SpawnSocketName = TEXT("WeaponSocket");
};
