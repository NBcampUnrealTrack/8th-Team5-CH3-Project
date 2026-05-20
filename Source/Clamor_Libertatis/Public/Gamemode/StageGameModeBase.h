// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StageGameModeBase.generated.h"

class AGameStateBase;



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

private:

	UFUNCTION()
	void HandleStageResult();

	UFUNCTION()
	void OnStageClear();

	UFUNCTION()
	void OnGameOver();

	void SaveInventoryToGameInstance();

	UPROPERTY()
	TObjectPtr<class AGameStateBase> CachedGameState;

	ECheckStageResult CurrentStatus;

	FTimerHandle ReturnToLobbyHandle;
};
