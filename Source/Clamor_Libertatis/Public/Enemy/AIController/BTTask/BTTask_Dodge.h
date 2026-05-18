#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Dodge.generated.h"

struct FDodgeTaskMemory
{
	bool bDodgeStarted;
	UAnimMontage* CachedDodgeMontage;
	float ElapsedTime;
};

UCLASS()
class CLAMOR_LIBERTATIS_API UBTTask_Dodge : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Dodge();

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FDodgeTaskMemory); }

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	// 마법사처럼 몽타주 없을 때 회피 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeDuration = 0.5f;
};