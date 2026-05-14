#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckChaseRange.generated.h"

struct FCheckChaseRangeMemory
{
	float MaxChasingDistance;
};

UCLASS()
class CLAMOR_LIBERTATIS_API UBTService_CheckChaseRange : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckChaseRange();

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FCheckChaseRangeMemory); }

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
