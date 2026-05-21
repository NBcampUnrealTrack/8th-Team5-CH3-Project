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
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void PostLogin(APlayerController* NewPlayer);

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
