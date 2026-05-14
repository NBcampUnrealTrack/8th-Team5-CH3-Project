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

	// 조회 기록 - 나중에 SaveGame 연동 시 이 함수들의 내부 구현만 교체
	void AddViewedQuestion(FName RowName) { ViewedQuestions.Add(RowName); }
	bool HasViewedQuestion(FName RowName) const { return ViewedQuestions.Contains(RowName); }
	const TSet<FName>& GetAllViewedQuestions() const { return ViewedQuestions; }

	// 임시 기능 -> Opening 1회만, 시나리오 매니저 업데이트 후 제거 예정
	bool bHasSeenOpening = false;

private:
	int32 WonBattleCount;

	TSet<FName> ViewedQuestions;
};
