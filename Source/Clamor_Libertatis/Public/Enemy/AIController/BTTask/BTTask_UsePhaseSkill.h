#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UsePhaseSkill.generated.h"

struct FUsePhaseSkillTaskMemory
{
	bool bShouldTrack;
	UAnimMontage* CachedSkillMontage;
};

UCLASS()
class CLAMOR_LIBERTATIS_API UBTTask_UsePhaseSkill : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_UsePhaseSkill();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sight")
	float MinTrackDistance;

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FUsePhaseSkillTaskMemory); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
