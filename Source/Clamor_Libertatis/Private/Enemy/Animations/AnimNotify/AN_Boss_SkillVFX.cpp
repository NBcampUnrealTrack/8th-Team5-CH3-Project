#include "Enemy/Animations/AnimNotify/AN_Boss_SkillVFX.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAN_Boss_SkillVFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(MeshComp->GetOwner());
	if (!Enemy) return;

	const FEnemySkillInfo* SkillInfo = Enemy->GetCurrentSkillInfo();
	if (!SkillInfo || !SkillInfo->SkillVFX) return;
	
	float AddDistanceToVFX = SkillInfo->AttackCollision.Distance;

	FVector SpawnLocation;
	if (SkillInfo->bIsHoming)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(Enemy->GetWorld(), 0);
		SpawnLocation = PlayerPawn ? PlayerPawn->GetActorLocation() : Enemy->GetActorLocation();
	}
	else if (MeshComp->DoesSocketExist(SpawnSocketName))
	{
		SpawnLocation = MeshComp->GetSocketLocation(SpawnSocketName);
	}
	else
	{
		FVector VFXSpawnLocation = Enemy->GetActorLocation() + Enemy->GetActorForwardVector() * AddDistanceToVFX;
		VFXSpawnLocation.Z = 0.f;
		SpawnLocation = VFXSpawnLocation;
	}
	SpawnLocation.Z = 0.f;

	FRotator SpawnRotation = MeshComp->DoesSocketExist(SpawnSocketName)
		? MeshComp->GetSocketRotation(SpawnSocketName)
		: Enemy->GetActorRotation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Enemy->GetWorld(), SkillInfo->SkillVFX, SpawnLocation, SpawnRotation);
}
