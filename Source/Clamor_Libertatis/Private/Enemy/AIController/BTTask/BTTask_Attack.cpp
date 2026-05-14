#include "Enemy/AIController/BTTask/BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/Boss/BossEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("NormalAttack");
	MaxDeltaYaw = 5.f;
	bAttackStarted = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	// 이전 실행에서 OnTaskFinished가 호출되지 않았을 경우를 대비해 항상 초기화
	bAttackStarted = false;
	CachedOwnerComp = nullptr;
	CachedAttackMontage = nullptr;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	// 이전 실행에서 델리게이트가 남아있을 수 있으므로 제거
	if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(AIC->GetPawn()))
	{
		if (UAnimInstance* AnimInst = MyOwner->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.RemoveDynamic(this, &UBTTask_Attack::OnAttackMontageEnded);
		}
	}

	APawn* OwnerPawn = AIC->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (!OwnerPawn || !TargetActor) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != CachedAttackMontage) return;

	if (CachedOwnerComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnAttackMontageEnded: FinishLatentTask"));
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	if (AAIController* AIC = OwnerComp.GetAIOwner())
	{
		if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(AIC->GetPawn()))
		{
			if (UAnimInstance* AnimInst = MyOwner->GetMesh()->GetAnimInstance())
			{
				AnimInst->OnMontageEnded.RemoveDynamic(this, &UBTTask_Attack::OnAttackMontageEnded);
			}
		}
	}

	bAttackStarted = false;
	CachedOwnerComp = nullptr;
	CachedAttackMontage = nullptr;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();

	if (!OwnerPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 공격이 이미 시작됐으면 몽타주가 끝날 때까지 TargetActor 소실 무시
	if (bAttackStarted)
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

	if (DeltaYaw <= MaxDeltaYaw && !bAttackStarted)
	{
		if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(OwnerPawn))
		{
			bAttackStarted = true;
			CachedOwnerComp = &OwnerComp;
			CachedAttackMontage = MyOwner->AttackToPlayer();

			if (ABossEnemy* Boss = Cast<ABossEnemy>(MyOwner))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("Count_NormalAttack"), Boss->GetNormalAttackCount());
			}

			if (CachedAttackMontage)
			{
				if (UAnimInstance* AnimInst = MyOwner->GetMesh()->GetAnimInstance())
				{
					AnimInst->OnMontageEnded.AddDynamic(this, &UBTTask_Attack::OnAttackMontageEnded);
				}
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
}
