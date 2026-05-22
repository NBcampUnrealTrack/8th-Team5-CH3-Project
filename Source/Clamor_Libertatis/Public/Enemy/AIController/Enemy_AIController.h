#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Enemy_AIController.generated.h"

struct FAIStimulus;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class CLAMOR_LIBERTATIS_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemy_AIController();
	
	void InitializeAIPerceptionComponent();
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AIPerception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AIPerception")
	TObjectPtr<UAISenseConfig_Sight> Sight_Config;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AIComponent")
	TObjectPtr<UBlackboardData> BB_BaseEnemy;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AIComponent")
	TObjectPtr<UBehaviorTree> BT_BaseEnemy;
	
	
	UFUNCTION()
	void OnTargetPerceived(AActor* Actor ,FAIStimulus Stimulus);
public:
	virtual void Tick(float DeltaTime) override;
};
