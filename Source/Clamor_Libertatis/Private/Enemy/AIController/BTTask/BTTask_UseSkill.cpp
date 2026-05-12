#include "Enemy/AIController/BTTask/BTTask_UseSkill.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/Boss/BossEnemy.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_UseSkill::UBTTask_UseSkill()
{
	NodeName = TEXT("UseSkill");
	MaxDeltaYaw = 5.f;
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = true;
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

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (!OwnerPawn || !TargetActor)
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
			Boss->SkillAttackToPlayer();
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("Count_NormalAttack"), 0);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bCanAttack"), false);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
}
