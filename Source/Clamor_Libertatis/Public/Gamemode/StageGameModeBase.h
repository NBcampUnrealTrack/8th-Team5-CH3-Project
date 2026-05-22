// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StageGameModeBase.generated.h"

class AGameStateBase;
class ABaseEnemy;



UENUM(BlueprintType)
enum class ECheckStageResult : uint8
{
	Win,
	Defeat,
	NotEnd
};



/**
 *
 */
UCLASS()
class  AStageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void PostLogin(APlayerController* NewPlayer);

	UPROPERTY(EditAnywhere, Category = "Dummy")
	TSubclassOf<ABaseEnemy> DummyEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Dummy")
	int32 DummyPoolSize = 5;

	UPROPERTY(EditAnywhere, Category = "Dummy")
	float DummySpawnRadius = 300.f;

private:
	UFUNCTION()
	void OnStageClear();

	UFUNCTION()
	void OnGameOver();

	void SaveInventoryToGameInstance();
	void LoadInventoryFromGameInstance(APlayerController* NewPlayer);

	UPROPERTY()
	TObjectPtr<class AGameStateBase> CachedGameState;

	ECheckStageResult CurrentStatus;

	FTimerHandle ReturnToLobbyHandle;
};
