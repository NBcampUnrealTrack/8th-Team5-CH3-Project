#include "Enemy/AIController/BTTask/BTTask_Dodge.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Boss/BossEnemy.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_Dodge::UBTTask_Dodge()
{
	NodeName = TEXT("DodgeBackward");
}

EBTNodeResult::Type UBTTask_Dodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	FDodgeTaskMemory* Memory = reinterpret_cast<FDodgeTaskMemory*>(NodeMemory);
	Memory->bDodgeStarted = false;
	Memory->CachedDodgeMontage = nullptr;
	Memory->ElapsedTime = 0.f;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	ABossEnemy* Boss = Cast<ABossEnemy>(AIC->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_Dodge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FDodgeTaskMemory* Memory = reinterpret_cast<FDodgeTaskMemory*>(NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	ABossEnemy* Boss = Cast<ABossEnemy>(AIC->GetPawn());
	if (!Boss)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 회피 진행 중: 몽타주 종료 대기
	if (Memory->bDodgeStarted)
	{
		if (Memory->CachedDodgeMontage)
		{
			// 검사/골렘: 루트모션 몽타주가 끝나면 완료
			UAnimInstance* AnimInst = Boss->GetMesh()->GetAnimInstance();
			if (!AnimInst || !AnimInst->Montage_IsPlaying(Memory->CachedDodgeMontage))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bShouldDodge"), false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		else
		{
			// 마법사: LaunchCharacter 이후 DodgeDuration 동안 대기
			Memory->ElapsedTime += DeltaSeconds;
			if (Memory->ElapsedTime >= DodgeDuration)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bShouldDodge"), false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
		return;
	}

	// 회피 시작 전: 플레이어 방향으로 점진적 회전 후 MaxDeltaYaw 범위 내에 들어오면 회피 실행
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(), TargetActor->GetActorLocation());
	FRotator TargetRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);

	FRotator NewRotation = FMath::RInterpTo(Boss->GetActorRotation(), TargetRotation, DeltaSeconds, 30.f);
	Boss->SetActorRotation(NewRotation);

	float DeltaYaw = FMath::Abs(FMath::FindDeltaAngleDegrees(Boss->GetActorRotation().Yaw, TargetRotation.Yaw));
	if (DeltaYaw <= MaxDeltaYaw)
	{
		Memory->CachedDodgeMontage = Boss->DodgeBackward();
		Memory->bDodgeStarted = true;
	}
}

void UBTTask_Dodge::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	FDodgeTaskMemory* Memory = reinterpret_cast<FDodgeTaskMemory*>(NodeMemory);
	Memory->bDodgeStarted = false;
	Memory->CachedDodgeMontage = nullptr;
	Memory->ElapsedTime = 0.f;
}