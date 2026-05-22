#include "Enemy/Animations/AnimNotify/AN_Mage_SpawnChargeProjectile.h"
#include "Enemy/Boss/BossEnemy_Mage.h"

void UAN_Mage_SpawnChargeProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ABossEnemy_Mage* Mage = Cast<ABossEnemy_Mage>(MeshComp->GetOwner());
	if (!Mage) return;

	Mage->SpawnChargeProjectile(SocketName);
}
