#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Gamemode/StageGameModeBase.h"
#include "StageManagerSubsystem.generated.h"

class ABaseEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);

UCLASS()
class UStageManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	UStageManagerSubsystem();

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UPROPERTY(BlueprintAssignable)
	FOnAllEnemiesDead OnAllEnemiesDead;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDead OnPlayerDead;

	void InitDummyPool(TArray<TSubclassOf<ABaseEnemy>> EnemyClasses, int32 PoolSize, const TArray<FTransform>& SpawnTransforms, float SpawnRadius = 300.f);

private:
	UFUNCTION()
	void OnBossHPChanged(float CurrentHP, float MaxHP);

	UFUNCTION()
	void OnPlayerDeath();

	UFUNCTION()
	void OnDummyHPChanged(float CurrentHP, float MaxHP);

	void BindDummyDeath(ABaseEnemy* Enemy);
	void RespawnDummy(ABaseEnemy* OldEnemy);
	FVector PickSpawnLocation(const TArray<FVector>& PlacedLocations) const;

	int32 RemainingBossCount;

	TArray<TObjectPtr<ABaseEnemy>> DummyPool;
	TArray<FTransform> DummySpawnTransforms;
	TArray<TObjectPtr<ABaseEnemy>> PendingRespawnDummies;
	float DummySpawnRadius = 300.f;
};
