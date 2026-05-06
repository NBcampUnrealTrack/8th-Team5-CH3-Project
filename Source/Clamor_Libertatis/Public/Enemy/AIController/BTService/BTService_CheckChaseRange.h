#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckChaseRange.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UBTService_CheckChaseRange : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckChaseRange();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	float MaxChasingDistance = 300.f;
};
