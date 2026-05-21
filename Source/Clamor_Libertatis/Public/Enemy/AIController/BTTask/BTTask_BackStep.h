#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BackStep.generated.h"

struct FBackStepTaskMemory
{
	float StepDirection; // 1.0 = 오른쪽, -1.0 = 왼쪽
	float ElapsedTime;
};

UCLASS()
class CLAMOR_LIBERTATIS_API UBTTask_BackStep : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_BackStep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SideStep")
	float SideStepDuration;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SpeedValue")
	float ScaleValue = 1.f;

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBackStepTaskMemory); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
