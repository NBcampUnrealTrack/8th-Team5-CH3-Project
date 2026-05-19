#include "Enemy/AIController/BTTask/BTTask_UseSkill.h"

#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Boss/BossEnemy.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_UseSkill::UBTTask_UseSkill()
{
	NodeName = TEXT("UseSkill");
	MaxDeltaYaw = 5.f;
	MinTrackDistance = 2.f;
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	FUseSkillTaskMemory* Memory = reinterpret_cast<FUseSkillTaskMemory*>(NodeMemory);
	Memory->bSkillStarted = false;
	Memory->CachedSkillMontage = nullptr;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* OwnerPawn = AIC->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (!OwnerPawn || !TargetActor) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_UseSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FUseSkillTaskMemory* Memory = reinterpret_cast<FUseSkillTaskMemory*>(NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();

	if (!OwnerPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Memory->bSkillStarted)
	{
		AActor* TrackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
		if (TrackTarget && FVector::Dist2D(OwnerPawn->GetActorLocation(), TrackTarget->GetActorLocation()) >= MinTrackDistance)
		{
			FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), TrackTarget->GetActorLocation());
			FRotator NewRot = FMath::RInterpTo(OwnerPawn->GetActorRotation(), FRotator(0.f, LookAt.Yaw, 0.f), DeltaSeconds, 30.f);
			OwnerPawn->SetActorRotation(NewRot);
		}

		if (ABossEnemy* Boss = Cast<ABossEnemy>(OwnerPawn))
		{
			UAnimInstance* AnimInst = Boss->GetMesh()->GetAnimInstance();
			if (!AnimInst || !AnimInst->Montage_IsPlaying(Memory->CachedSkillMontage))
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		return;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), TargetActor->GetActorLocation());
	FRotator TargetRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);

	FRotator NewRotation = FMath::RInterpTo(OwnerPawn->GetActorRotation(), TargetRotation, DeltaSeconds, 30.f);
	OwnerPawn->SetActorRotation(NewRotation);

	float DeltaYaw = FMath::Abs(FMath::FindDeltaAngleDegrees(OwnerPawn->GetActorRotation().Yaw, TargetRotation.Yaw));

	if (DeltaYaw <= MaxDeltaYaw)
	{
		if (ABossEnemy* Boss = Cast<ABossEnemy>(OwnerPawn))
		{
			Memory->CachedSkillMontage = Boss->SkillAttackToPlayer();
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("Count_NormalAttack"), 0);

			if (Memory->CachedSkillMontage)
			{
				Memory->bSkillStarted = true;
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}

void UBTTask_UseSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	FUseSkillTaskMemory* Memory = reinterpret_cast<FUseSkillTaskMemory*>(NodeMemory);
	Memory->bSkillStarted = false;
	Memory->CachedSkillMontage = nullptr;
}
