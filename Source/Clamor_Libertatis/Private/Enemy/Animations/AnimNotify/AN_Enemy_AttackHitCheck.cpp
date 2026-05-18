#include "Enemy/Animations/AnimNotify/AN_Enemy_AttackHitCheck.h"

#include "Enemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"

void UAN_Enemy_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(MeshComp->GetOwner()))
	{
		MyOwner->AttackHitCheck();
	}
}
