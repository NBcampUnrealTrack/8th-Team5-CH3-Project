#include "Enemy/Animations/AnimNotify/AN_Boss_SkillVFX.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "NiagaraFunctionLibrary.h"

void UAN_Boss_SkillVFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(MeshComp->GetOwner());
	if (!Enemy) return;

	const FEnemySkillInfo* SkillInfo = Enemy->GetCurrentSkillInfo();
	if (!SkillInfo || !SkillInfo->SkillVFX) return;

	FVector SpawnLocation = MeshComp->DoesSocketExist(SpawnSocketName)
		? MeshComp->GetSocketLocation(SpawnSocketName)
		: Enemy->GetActorLocation();

	FRotator SpawnRotation = MeshComp->DoesSocketExist(SpawnSocketName)
		? MeshComp->GetSocketRotation(SpawnSocketName)
		: Enemy->GetActorRotation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Enemy->GetWorld(), SkillInfo->SkillVFX, SpawnLocation, SpawnRotation);
}
