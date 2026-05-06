#include "Enemy/AIController/BTService/BTService_CheckChaseRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckChaseRange::UBTService_CheckChaseRange()
{
	NodeName = TEXT("CheckChaseRange");
	Interval = 0.2f;
}

void UBTService_CheckChaseRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIC->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	
	if (OwnerPawn && TargetActor)
	{
		float Dist = FVector::Dist(OwnerPawn->GetActorLocation(), TargetActor->GetActorLocation());
		if (Dist <= MaxChasingDistance)
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
