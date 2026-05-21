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

void UCLGameInstance::ResetGame()
{
    WonBattleCount = 0;
    LastScenarioRowName = FName("Scenario_1");
    ViewedQuestions.Empty();
    bHasSelectedQuestion = false;
    SavedInventorySlots.Empty();
}


void UCLGameInstance::SaveInventory(const TArray<FInventorySlot>& Slots)
{
    SavedInventorySlots = Slots;
    UE_LOG(LogTemp, Log, TEXT("[CLGameInstance] 인벤토리 저장 완료: %d 슬롯"), Slots.Num());
}

