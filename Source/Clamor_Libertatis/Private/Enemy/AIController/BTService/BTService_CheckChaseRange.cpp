#include "Enemy/AIController/BTService/BTService_CheckChaseRange.h"

UBTService_CheckChaseRange::UBTService_CheckChaseRange()
{
	NodeName = TEXT("CheckChaseRange");
	Interval = 0.2f;
}

void UBTService_CheckChaseRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
