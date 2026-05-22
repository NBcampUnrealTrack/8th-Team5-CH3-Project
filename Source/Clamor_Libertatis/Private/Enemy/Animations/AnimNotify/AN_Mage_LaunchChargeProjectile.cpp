#include "Enemy/Animations/AnimNotify/AN_Mage_LaunchChargeProjectile.h"
#include "Enemy/Boss/BossEnemy_Mage.h"

void UAN_Mage_LaunchChargeProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ABossEnemy_Mage* Mage = Cast<ABossEnemy_Mage>(MeshComp->GetOwner());
	if (!Mage) return;

	Mage->LaunchChargeProjectile();
}
