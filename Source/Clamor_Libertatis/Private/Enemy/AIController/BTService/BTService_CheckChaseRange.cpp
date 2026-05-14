#include "Enemy/AIController/BTService/BTService_CheckChaseRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"

UBTService_CheckChaseRange::UBTService_CheckChaseRange()
{
	NodeName = TEXT("CheckChaseRange");
	Interval = 0.2f;
	bNotifyBecomeRelevant = true;
}

void UBTService_CheckChaseRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	FCheckChaseRangeMemory* Memory = reinterpret_cast<FCheckChaseRangeMemory*>(NodeMemory);
	Memory->MaxChasingDistance = 0.f;

	if (AAIController* AIC = OwnerComp.GetAIOwner())
	{
		if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(AIC->GetPawn()))
		{
			Memory->MaxChasingDistance = MyOwner->GetEnemyStatComp()->GetEnemyStat().MaxChasingDistance;
		}
	}
}

void UBTService_CheckChaseRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FCheckChaseRangeMemory* Memory = reinterpret_cast<FCheckChaseRangeMemory*>(NodeMemory);

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

	if (OwnerPawn && TargetActor)
	{
		float Dist = FVector::Dist(OwnerPawn->GetActorLocation(), TargetActor->GetActorLocation());
		if (Dist <= Memory->MaxChasingDistance)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bInRange"),true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bCanAttack"),true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bInRange"),false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bCanAttack"),false);
		}
	}
}
