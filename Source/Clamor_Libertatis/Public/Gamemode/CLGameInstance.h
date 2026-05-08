// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CLGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCLGameInstance();

	void AddWonBattle();
	int32 GetWonBattleCount() const { return WonBattleCount; }

private:
	int32 WonBattleCount;
};
