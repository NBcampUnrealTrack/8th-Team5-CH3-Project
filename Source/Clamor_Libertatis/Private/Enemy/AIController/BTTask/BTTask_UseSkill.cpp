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
	bSkillStarted = false;
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	bSkillStarted = false;
	CachedOwnerComp = nullptr;
	CachedSkillMontage = nullptr;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	if (ABossEnemy* Boss = Cast<ABossEnemy>(AIC->GetPawn()))
	{
		if (UAnimInstance* AnimInst = Boss->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.RemoveDynamic(this, &UBTTask_UseSkill::OnSkillMontageEnded);
		}
	}

	APawn* OwnerPawn = AIC->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (!OwnerPawn || !TargetActor) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_UseSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();

	if (!OwnerPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 스킬이 이미 시작됐으면 몽타주가 끝날 때까지 TargetActor 소실 무시
	if (bSkillStarted)
	{
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

	if (DeltaYaw <= MaxDeltaYaw && !bSkillStarted)
	{
		if (ABossEnemy* Boss = Cast<ABossEnemy>(OwnerPawn))
		{
			bSkillStarted = true;
			CachedOwnerComp = &OwnerComp;
			CachedSkillMontage = Boss->SkillAttackToPlayer();
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("Count_NormalAttack"), 0);

			if (CachedSkillMontage)
			{
				if (UAnimInstance* AnimInst = Boss->GetMesh()->GetAnimInstance())
				{
					AnimInst->OnMontageEnded.AddDynamic(this, &UBTTask_UseSkill::OnSkillMontageEnded);
				}
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

void UBTTask_UseSkill::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != CachedSkillMontage) return;

	if (CachedOwnerComp)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_UseSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (AAIController* AIC = OwnerComp.GetAIOwner())
	{
		if (ABossEnemy* Boss = Cast<ABossEnemy>(AIC->GetPawn()))
		{
			if (UAnimInstance* AnimInst = Boss->GetMesh()->GetAnimInstance())
			{
				AnimInst->OnMontageEnded.RemoveDynamic(this, &UBTTask_UseSkill::OnSkillMontageEnded);
			}
		}
	}

	bSkillStarted = false;
	CachedOwnerComp = nullptr;
	CachedSkillMontage = nullptr;
}
