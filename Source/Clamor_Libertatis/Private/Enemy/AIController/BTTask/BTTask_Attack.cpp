#include "Enemy/AIController/BTTask/BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("NormalAttack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	
	if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(AIC->GetPawn()))
	{
		MyOwner->AttackToPlayer();
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bCanAttack"),false);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
