#include "Enemy/AIController/BTTask/BTTask_BackStep.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_BackStep::UBTTask_BackStep()
{
	NodeName = TEXT("SideStep");
	SideStepDuration = 1.5f;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_BackStep::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;

	FBackStepTaskMemory* Memory = reinterpret_cast<FBackStepTaskMemory*>(NodeMemory);
	Memory->StepDirection = FMath::RandBool() ? 1.f : -1.f;
	Memory->ElapsedTime = 0.f;

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	if (!AIC->GetPawn()) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_BackStep::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBackStepTaskMemory* Memory = reinterpret_cast<FBackStepTaskMemory*>(NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();
	if (!OwnerPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 플레이어 방향으로 회전 유지
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), TargetActor->GetActorLocation());
	FRotator NewRot = FMath::RInterpTo(OwnerPawn->GetActorRotation(), FRotator(0.f, LookAt.Yaw, 0.f), DeltaSeconds, 10.f);
	OwnerPawn->SetActorRotation(NewRot);

	// 플레이어 방향 기준 좌/우 스트레이프
	FVector ToPlayer = (TargetActor->GetActorLocation() - OwnerPawn->GetActorLocation()).GetSafeNormal2D();
	FVector StrafeDir = FVector::CrossProduct(FVector::UpVector, ToPlayer) * Memory->StepDirection;
	OwnerPawn->AddMovementInput(StrafeDir, 1.f);

	Memory->ElapsedTime += DeltaSeconds;
	if (Memory->ElapsedTime >= SideStepDuration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_BackStep::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bCanPatrol"), false);
}
