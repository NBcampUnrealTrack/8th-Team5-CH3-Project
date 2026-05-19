#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UseSkill.generated.h"

struct FUseSkillTaskMemory
{
	bool bSkillStarted;
	UAnimMontage* CachedSkillMontage;
};

UCLASS()
class CLAMOR_LIBERTATIS_API UBTTask_UseSkill : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_UseSkill();

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FUseSkillTaskMemory); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sight")
	float MaxDeltaYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sight")
	float MinTrackDistance;
};
