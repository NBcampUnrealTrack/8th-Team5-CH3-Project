#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Chase.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UBTTask_Chase : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Chase();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
