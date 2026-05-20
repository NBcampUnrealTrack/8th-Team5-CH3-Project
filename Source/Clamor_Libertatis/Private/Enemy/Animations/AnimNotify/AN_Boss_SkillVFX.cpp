#include "Enemy/Animations/AnimNotify/AN_Boss_SkillVFX.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/DataTable/DA_BaseEnemySkill.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UAN_Boss_SkillVFX::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(MeshComp->GetOwner());
	if (!Enemy) return;

	const FEnemySkillInfo* SkillInfo = Enemy->GetCurrentSkillInfo();
	if (!SkillInfo || !SkillInfo->SkillVFX) return;
	
	float AddDistanceToVFX = SkillInfo->AttackCollision.Distance;

	auto GetFeetLocation = [](ACharacter* Char) -> FVector
	{
		return Char->GetActorLocation() - FVector(0.f, 0.f, Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	};

	FVector SpawnLocation;
	if (SkillInfo->bIsHoming)
	{
		ACharacter* PlayerChar = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(Enemy->GetWorld(), 0));
		SpawnLocation = PlayerChar ? GetFeetLocation(PlayerChar) : GetFeetLocation(Enemy);
	}
	else if (MeshComp->DoesSocketExist(SpawnSocketName))
	{
		SpawnLocation = MeshComp->GetSocketLocation(SpawnSocketName);
	}
	else
	{
		SpawnLocation = GetFeetLocation(Enemy) + Enemy->GetActorForwardVector() * AddDistanceToVFX;
	}
	

	FRotator SpawnRotation = MeshComp->DoesSocketExist(SpawnSocketName)
		? MeshComp->GetSocketRotation(SpawnSocketName)
		: Enemy->GetActorRotation();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(Enemy->GetWorld(), SkillInfo->SkillVFX, SpawnLocation, SpawnRotation);
}
