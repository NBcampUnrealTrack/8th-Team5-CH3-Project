#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Enemy_AIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;

UCLASS()
class CLAMOR_LIBERTATIS_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemy_AIController();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AIPerception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AIPerception")
	TObjectPtr<UAISenseConfig_Sight> Sight_Config;
	
	UFUNCTION()
	void OnTargetPerceived(AActor* Actor ,FAIStimulus Stimulus);
public:
	virtual void Tick(float DeltaTime) override;
};
