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
	MinTrackDistance = 2.f;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	FAttackTaskMemory* Memory = reinterpret_cast<FAttackTaskMemory*>(NodeMemory);
	Memory->bAttackStarted = false;
	Memory->bShouldTrack = true;
	Memory->CachedAttackMontage = nullptr;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* OwnerPawn = AIC->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (!OwnerPawn || !TargetActor) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FAttackTaskMemory* Memory = reinterpret_cast<FAttackTaskMemory*>(NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();

	if (!OwnerPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Memory->bAttackStarted)
	{
		if (Memory->bShouldTrack)
		{
			AActor* TrackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
			if (TrackTarget && FVector::Dist2D(OwnerPawn->GetActorLocation(), TrackTarget->GetActorLocation()) >= MinTrackDistance)
			{
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), TrackTarget->GetActorLocation());
				FRotator NewRot = FMath::RInterpTo(OwnerPawn->GetActorRotation(), FRotator(0.f, LookAt.Yaw, 0.f), DeltaSeconds, 30.f);
				OwnerPawn->SetActorRotation(NewRot);
			}
		}

		if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(OwnerPawn))
		{
			UAnimInstance* AnimInst = MyOwner->GetMesh()->GetAnimInstance();
			if (!AnimInst || !AnimInst->Montage_IsPlaying(Memory->CachedAttackMontage))
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
		if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(OwnerPawn))
		{
			Memory->CachedAttackMontage = MyOwner->AttackToPlayer();

			if (ABossEnemy* Boss = Cast<ABossEnemy>(MyOwner))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("Count_NormalAttack"), Boss->GetNormalAttackCount());
			}

			if (Memory->CachedAttackMontage)
			{
				Memory->bAttackStarted = true;
				if (const FEnemySkillInfo* SkillInfo = MyOwner->GetCurrentSkillInfo())
				{
					Memory->bShouldTrack = SkillInfo->bTrackPlayerDuringAttack;
				}
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	FAttackTaskMemory* Memory = reinterpret_cast<FAttackTaskMemory*>(NodeMemory);
	Memory->bAttackStarted = false;
	Memory->CachedAttackMontage = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("[BTTask_Attack] OnTaskFinished - TaskResult: %d (Succeeded=1, Failed=0, Aborted=2)"), (int32)TaskResult);

	if (TaskResult == EBTNodeResult::Succeeded)
	{
		UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_Attack] Succeeded 진입 - BlackboardComponent 유효: %s"), BB ? TEXT("true") : TEXT("false"));
		if (BB)
		{
			BB->SetValueAsBool(TEXT("bCanPatrol"), true);
			UE_LOG(LogTemp, Warning, TEXT("[BTTask_Attack] bCanPatrol 설정 후 값: %s"), BB->GetValueAsBool(TEXT("bCanPatrol")) ? TEXT("true") : TEXT("false"));
		}
	}
}
