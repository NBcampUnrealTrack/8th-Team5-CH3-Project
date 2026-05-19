#include "Enemy/AIController/BTTask/BTTask_UsePhaseSkill.h"

#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Boss/BossEnemy.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_UsePhaseSkill::UBTTask_UsePhaseSkill()
{
	NodeName = TEXT("UsePhaseSkill");
	MinTrackDistance = 2.f;
}

EBTNodeResult::Type UBTTask_UsePhaseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	FUsePhaseSkillTaskMemory* Memory = reinterpret_cast<FUsePhaseSkillTaskMemory*>(NodeMemory);
	Memory->bShouldTrack = true;
	Memory->CachedSkillMontage = nullptr;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	ABossEnemy* Boss = Cast<ABossEnemy>(AIC->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	UAnimMontage* Montage = Boss->PhaseSkillAttackToPlayer();
	if (!Montage) return EBTNodeResult::Failed;

	Memory->CachedSkillMontage = Montage;
	if (const FEnemySkillInfo* SkillInfo = Boss->GetCurrentSkillInfo())
	{
		Memory->bShouldTrack = SkillInfo->bTrackPlayerDuringAttack;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("Count_NormalAttack"), 0);

	return EBTNodeResult::InProgress;
}

void UBTTask_UsePhaseSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FUsePhaseSkillTaskMemory* Memory = reinterpret_cast<FUsePhaseSkillTaskMemory*>(NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	ABossEnemy* Boss = Cast<ABossEnemy>(AIC->GetPawn());
	if (!Boss)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Memory->bShouldTrack)
	{
		AActor* TrackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
		if (TrackTarget && FVector::Dist2D(Boss->GetActorLocation(), TrackTarget->GetActorLocation()) >= MinTrackDistance)
		{
			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), TrackTarget->GetActorLocation());
			FRotator NewRot = FMath::RInterpTo(Boss->GetActorRotation(), FRotator(0.f, LookAt.Yaw, 0.f), DeltaSeconds, 30.f);
			Boss->SetActorRotation(NewRot);
		}
	}

	UAnimInstance* AnimInst = Boss->GetMesh()->GetAnimInstance();
	if (!AnimInst || !AnimInst->Montage_IsPlaying(Memory->CachedSkillMontage))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsPhase2"), false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

