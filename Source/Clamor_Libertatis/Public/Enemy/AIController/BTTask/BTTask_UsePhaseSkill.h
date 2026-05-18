#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UsePhaseSkill.generated.h"

struct FUsePhaseSkillTaskMemory
{
	UAnimMontage* CachedSkillMontage;
};

UCLASS()
class CLAMOR_LIBERTATIS_API UBTTask_UsePhaseSkill : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_UsePhaseSkill();

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FUsePhaseSkillTaskMemory); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
