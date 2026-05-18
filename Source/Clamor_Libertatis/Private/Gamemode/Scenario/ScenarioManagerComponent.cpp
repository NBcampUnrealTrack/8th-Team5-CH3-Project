// Fill out your copyright notice in the Description page of Project Settings.

#include "Gamemode/Scenario/ScenarioManagerComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Gamemode/CLGameInstance.h"


UScenarioManagerComponent::UScenarioManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when Scenario starts First time or when Next Step is requested
void UScenarioManagerComponent::StartScenario(FName RowName)
{
    CurrentRowName = RowName;
    bScenarioEnd = false;

    FString Left, Right;
    if (SplitRowNameFromEnd(RowName, Left, Right))
    {
        if (Right == TEXT("2") && Left.StartsWith(TEXT("Question")) && Left != TEXT("Question_6"))
        {
            if (UCLGameInstance* GI = Cast<UCLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            {
                GI->RegisterViewedQuestion(Left);
            }
        }
    }

    if (!CachedDialogueWidget && DialogueWidgetClass)
    {

        // TODO:: Opening Widget 종료시 생성쪽이 더 올바름. 일단 임시
        CachedDialogueWidget = CreateWidget<UUserWidget>(GetWorld(), DialogueWidgetClass);
        if (CachedDialogueWidget)
        {
            CachedDialogueWidget->AddToViewport(98);            
        }
    }

    UpdateDisplay();
}

void UScenarioManagerComponent::RequestNextStep()
{
    if (!ScenarioTable || CurrentRowName.IsNone())
    {
        HandleScenarioEnd();
        return;
    }
    FScenarioData* CurrentData = ScenarioTable->FindRow<FScenarioData>(CurrentRowName, TEXT(""));
    if (!CurrentData)
    {
        HandleScenarioEnd();
        return;
    }

    FName NextRowName;

    if (CurrentData->NextID.IsNone())
    {
        FString Left, Right;
        if (SplitRowNameFromEnd(CurrentRowName, Left, Right))
        {
            int32 NextNum = FCString::Atoi(*Right) + 1;
            NextRowName = FName(*FString::Printf(TEXT("%s_%d"), *Left, NextNum));
        }
        else
        {            
            HandleScenarioEnd();
            return;
        }
    }
    else
    {
        NextRowName = CurrentData->NextID;
    }

    if (NextRowName == FName("End"))
    {
        HandleScenarioEnd();
        return;
    }

    SaveLastRowName(CurrentRowName);

    FScenarioData* NextData = ScenarioTable->FindRow<FScenarioData>(NextRowName, TEXT(""));
    if (NextData)
    {
        CurrentRowName = NextRowName;
        OnScenarioStepUpdated.Broadcast(*NextData, BuildChoiceList(*NextData));
    }
    else
    {
        HandleScenarioEnd();
    }
}

bool UScenarioManagerComponent::SplitRowNameFromEnd(FName InRowName, FString& OutLeft, FString& OutRight) const
{
    if (InRowName.IsNone())
    {
        return false;
    }

    return InRowName.ToString().Split(TEXT("_"), &OutLeft, &OutRight, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
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
        OnScenarioStepUpdated.Broadcast(*Data, BuildChoiceList(*Data));
        SaveLastRowName(CurrentRowName);
    }
}

TArray<FScenarioData> UScenarioManagerComponent::BuildChoiceList(const FScenarioData& Data) const
{
    TArray<FScenarioData> Result;

    for (const FName& ChoiceID : Data.ChoiceIDs)
    {
        if (FScenarioData* ChoiceData = ScenarioTable->FindRow<FScenarioData>(ChoiceID, TEXT("")))
        {
            Result.Add(*ChoiceData);            
        }
    }
    return Result;
}

void UScenarioManagerComponent::SaveLastRowName(FName RowName)
{
    if (UCLGameInstance* GI = GetWorld()->GetGameInstance<UCLGameInstance>())
    {
        GI->LastScenarioRowName = RowName;        
    }
}

void UScenarioManagerComponent::HandleScenarioEnd()
{    
    OnScenarioEnded.Broadcast();
    bScenarioEnd = true;
}

bool UScenarioManagerComponent::IsStepAlreadyReadQuestion(FName TargetRowName) const
{
    FString Left, Right;
    if (SplitRowNameFromEnd(TargetRowName, Left, Right))
    {
        if (Right.Equals(TEXT("2")) && Left.StartsWith(TEXT("Question")))
        {
            if (UCLGameInstance* GI = Cast<UCLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
            {
                return GI->HasViewedQuestion(FName(*Left));
            }
        }
    }
    return false;
}

bool UScenarioManagerComponent::GetSkipQuestion(FScenarioData& Output) const
{
    FScenarioData* SkipQuestion = ScenarioTable->FindRow<FScenarioData>(FName("Question_6_1"), TEXT(""));
    if (SkipQuestion)
    {
        Output = *SkipQuestion;
        return true;
    }

    
    return false;
}

bool UScenarioManagerComponent::IsScenarioEnd() const
{
    return bScenarioEnd;
}
