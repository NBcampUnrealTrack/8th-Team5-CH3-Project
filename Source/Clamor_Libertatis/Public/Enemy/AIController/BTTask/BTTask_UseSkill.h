#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UseSkill.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UBTTask_UseSkill : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_UseSkill();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sight")
	float MaxDeltaYaw;
};
