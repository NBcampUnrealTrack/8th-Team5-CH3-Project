#include "Gamemode/CLGameInstance.h"

UCLGameInstance::UCLGameInstance()
{
	WonBattleCount = 0;
	LastScenarioRowName = TEXT("Scenario_1");    
}

void UCLGameInstance::AddWonBattle()
{
	WonBattleCount++;
}

bool UCLGameInstance::HasWatchedOpening() const
{
	return LastScenarioRowName != FName(TEXT("Scenario_1"));
}

void UCLGameInstance::RegisterViewedQuestion(const FString& PrefixKey)
{
    FName KeyName = FName(*PrefixKey);

    if (!ViewedQuestions.Contains(KeyName))
    {
        ViewedQuestions.Add(KeyName);
        UE_LOG(LogTemp, Log, TEXT("[CLGameInstance] 질문 기록 완료: %s"), *PrefixKey);
    }
}
