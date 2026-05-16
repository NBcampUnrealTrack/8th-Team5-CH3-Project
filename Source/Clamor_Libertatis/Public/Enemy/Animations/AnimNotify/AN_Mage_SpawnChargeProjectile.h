#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Mage_SpawnChargeProjectile.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UAN_Mage_SpawnChargeProjectile : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category="Projectile")
	FName SocketName = TEXT("MagicSocket");
};
