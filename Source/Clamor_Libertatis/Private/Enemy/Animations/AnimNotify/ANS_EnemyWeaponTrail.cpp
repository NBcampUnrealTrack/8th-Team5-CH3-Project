#include "Enemy/Animations/AnimNotify/ANS_EnemyWeaponTrail.h"

#include "Enemy/BaseEnemy.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UANS_EnemyWeaponTrail::UANS_EnemyWeaponTrail()
{
	SocketName = TEXT("hand_r");
	bDestroyAtEnd = false;
}

UFXSystemComponent* UANS_EnemyWeaponTrail::SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const
{
	ABaseEnemy* Enemy = MeshComp ? Cast<ABaseEnemy>(MeshComp->GetOwner()) : nullptr;
	if (!Enemy) return Super::SpawnEffect(MeshComp, Animation); // 프리뷰 환경 폴백

	const FEnemySkillInfo* SkillInfo = Enemy->GetCurrentSkillInfo();
	if (!SkillInfo || !SkillInfo->SkillVFX) return nullptr;

	FName SpawnSocket = MeshComp->DoesSocketExist(SocketName) ? SocketName : NAME_None;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		SkillInfo->SkillVFX,
		MeshComp,
		SpawnSocket,
		LocationOffset,
		RotationOffset,
		EAttachLocation::KeepRelativeOffset,
		true,
		true,
		ENCPoolMethod::None,
		true
	);

	if (NiagaraComp)
	{
		NiagaraComp->ComponentTags.AddUnique(GetSpawnedComponentTag());
		if (bApplyRateScaleAsTimeDilation && Animation)
		{
			NiagaraComp->SetCustomTimeDilation(Animation->RateScale);
		}
	}

	return NiagaraComp;
}
