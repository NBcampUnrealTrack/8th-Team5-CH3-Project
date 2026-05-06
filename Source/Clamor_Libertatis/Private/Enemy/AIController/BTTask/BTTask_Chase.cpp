#include "Enemy/AIController/BTTask/BTTask_Chase.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = TEXT("Chase_Player");
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Failed;
}
