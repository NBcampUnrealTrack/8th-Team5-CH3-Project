#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StageManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);

UCLASS()
class UStageManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	UStageManagerSubsystem();

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UPROPERTY(BlueprintAssignable)
	FOnAllEnemiesDead OnAllEnemiesDead;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDead OnPlayerDead;

private:
	UFUNCTION()
	void OnEnemyDeath();

	UFUNCTION()
	void OnPlayerDeath();

	int32 RemainingEnemyCount;
};
