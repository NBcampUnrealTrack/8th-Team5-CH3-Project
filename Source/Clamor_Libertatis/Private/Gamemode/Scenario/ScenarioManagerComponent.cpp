// Fill out your copyright notice in the Description page of Project Settings.

#include "Gamemode/Scenario/ScenarioManagerComponent.h"
#include "Blueprint/UserWidget.h"

UScenarioManagerComponent::UScenarioManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScenarioManagerComponent::StartScenario(FName RowName)
{
    CurrentRowName = RowName;

    if (!CachedDialogueWidget && DialogueWidgetClass)
    {

        // TODO:: Opening Widget 종료시 생성쪽이 더 올바름. 일단 임시
        CachedDialogueWidget = CreateWidget<UUserWidget>(GetWorld(), DialogueWidgetClass);
        if (CachedDialogueWidget)
        {
            CachedDialogueWidget->AddToViewport(98);
            UE_LOG(LogTemp, Warning, TEXT("[ScenarioManager] Created Dialogue Widget"));                        
        }
    }

    UpdateDisplay();
}

void UScenarioManagerComponent::RequestNextStep()
{
    if (!ScenarioTable || CurrentRowName.IsNone())
    {
        OnScenarioEnded.Broadcast();
        return;
    }

    FScenarioData* CurrentData = ScenarioTable->FindRow<FScenarioData>(CurrentRowName, TEXT(""));
    if (!CurrentData)
    {
        OnScenarioEnded.Broadcast();
        return;
    }

    FName NextRowName;

    if (CurrentData->NextID.IsNone())
    {
        FString Left, Right;
        if (CurrentRowName.ToString().Split(TEXT("_"), &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
        {
            int32 NextNum = FCString::Atoi(*Right) + 1;
            NextRowName = FName(*FString::Printf(TEXT("%s_%d"), *Left, NextNum));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[ScenarioManager] Not Allowed RowName : %s"), *CurrentRowName.ToString());
            OnScenarioEnded.Broadcast();
            return;
        }
    }
    else
    {
        NextRowName = CurrentData->NextID;
    }

    FScenarioData* NextData = ScenarioTable->FindRow<FScenarioData>(NextRowName, TEXT(""));
    if (NextData)
    {
        CurrentRowName = NextRowName;
        CachedChoiceList = BuildChoiceList(*NextData);
        UE_LOG(LogTemp, Warning, TEXT("[ScenarioManager] Broadcast 직전 CachedChoiceList: %d개"), CachedChoiceList.Choices.Num());
        OnScenarioStepUpdated.Broadcast(*NextData);
        UE_LOG(LogTemp, Warning, TEXT("[ScenarioManager] Broadcast 직후 CachedChoiceList: %d개"), CachedChoiceList.Choices.Num());
    }
    else
    {
        OnScenarioEnded.Broadcast();
    }
}

void UScenarioManagerComponent::UpdateDisplay()
{
    if (!ScenarioTable || CurrentRowName.IsNone())
    {
        return;
    }

    FScenarioData* Data = ScenarioTable->FindRow<FScenarioData>(CurrentRowName, TEXT(""));
    if (Data)
    {
        CachedChoiceList = BuildChoiceList(*Data);
        OnScenarioStepUpdated.Broadcast(*Data);
    }
}

FScenarioChoiceList UScenarioManagerComponent::BuildChoiceList(const FScenarioData& Data) const
{
    FScenarioChoiceList Result;
    for (const FName& ChoiceID : Data.ChoiceIDs)
    {
        if (FScenarioData* ChoiceData = ScenarioTable->FindRow<FScenarioData>(ChoiceID, TEXT("")))
        {
            FScenarioChoiceEntry Entry;
            Entry.Dialogue = ChoiceData->Dialogue;
            Entry.RowName  = ChoiceID;
            Result.Choices.Add(Entry);
            UE_LOG(LogTemp, Warning, TEXT("[ScenarioManager] %s 선택지 추가"), *ChoiceData->Dialogue.ToString());
        }
    }
    return Result;
}
