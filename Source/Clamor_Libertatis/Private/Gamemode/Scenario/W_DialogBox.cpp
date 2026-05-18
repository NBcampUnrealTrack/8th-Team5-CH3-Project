// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/Scenario/W_DialogBox.h"
#include "Gamemode/LobbyGameModeBase.h"
#include "Gamemode/Scenario/ScenarioManagerComponent.h"
#include "Gamemode/Scenario/W_Button.h"

#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"


UW_DialogBox::UW_DialogBox(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bClickNext(true)
{
}

ALobbyGameModeBase* UW_DialogBox::GetLobbyGameMode() const
{
    return Cast<ALobbyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

TArray<FScenarioData> UW_DialogBox::GetRandomChoices(const TArray<FScenarioData>& AllChoices, int32 Count) const
{
    TArray<FScenarioData> Shuffled = AllChoices;
    for (int32 i = Shuffled.Num() - 1; i > 0; --i)
    {
        Shuffled.Swap(i, FMath::RandRange(0, i));
    }
    Shuffled.SetNum(FMath::Min(Count, Shuffled.Num()));
    return Shuffled;
}

void UW_DialogBox::AddChoiceButton(const FScenarioData& ChoiceData, UScenarioManagerComponent* Manager)
{
    UW_Button* NewButtonWidget = CreateWidget<UW_Button>(this, ButtonWidgetClass);
    if (!NewButtonWidget)
    {
        return;
    }

    NewButtonWidget->ButtonText = ChoiceData.Dialogue;
    NewButtonWidget->NextID = ChoiceData.NextID;
    NewButtonWidget->bIsRead = Manager->IsStepAlreadyReadQuestion(ChoiceData.NextID);

    NewButtonWidget->OnChoiceClicked.RemoveDynamic(Manager, &UScenarioManagerComponent::StartScenario);
    NewButtonWidget->OnChoiceClicked.AddDynamic(Manager, &UScenarioManagerComponent::StartScenario);

    VBox_Choices->AddChildToVerticalBox(NewButtonWidget);
}

void UW_DialogBox::NativeConstruct()
{
    Super::NativeConstruct();

    ALobbyGameModeBase* LobbyGameMode = GetLobbyGameMode();
    if (LobbyGameMode)
    {
        LobbyGameMode->ScenarioManagerComp->OnScenarioEnded.RemoveDynamic(this, &UW_DialogBox::HandleScenarioEnded);
        LobbyGameMode->ScenarioManagerComp->OnScenarioStepUpdated.RemoveDynamic(this, &UW_DialogBox::HandleScenarioStepUpdated);

        LobbyGameMode->ScenarioManagerComp->OnScenarioEnded.AddDynamic(this, &UW_DialogBox::HandleScenarioEnded);
        LobbyGameMode->ScenarioManagerComp->OnScenarioStepUpdated.AddDynamic(this, &UW_DialogBox::HandleScenarioStepUpdated);
    }
}

void UW_DialogBox::HandleScenarioEnded()
{
    RemoveFromParent();
}

void UW_DialogBox::HandleScenarioStepUpdated(const FScenarioData& MainData, const TArray<FScenarioData>& Choices)
{
    GetWorld()->GetTimerManager().ClearTimer(DelayChoiceTimerHandler);

    if (Txt_Dialog)
    {
        Txt_Dialog->SetText(MainData.Dialogue);
    }

    ALobbyGameModeBase* LobbyGameMode = GetLobbyGameMode();
    if (!LobbyGameMode || !LobbyGameMode->ScenarioManagerComp)
    {
        return;
    }

    if (!ButtonWidgetClass || !VBox_Choices)
    {
        return;
    }

    UScenarioManagerComponent* Manager = LobbyGameMode->ScenarioManagerComp;

    bClickNext = Choices.Num() == 0;
    VBox_Choices->ClearChildren();

    if (MainData.Type == ETalkType::Question)
    {
        const TArray<FScenarioData>& RandomChoices = GetRandomChoices(Choices, 3);
        for (const FScenarioData& ChoiceData : RandomChoices)
        {
            AddChoiceButton(ChoiceData, Manager);
        }

        GetWorld()->GetTimerManager().SetTimer(
            DelayChoiceTimerHandler,
            this,
            &UW_DialogBox::ShowHiddenButton,
            5.0f,
            false
        );
    }
    else
    {
        for (const FScenarioData& ChoiceData : Choices)
        {
            AddChoiceButton(ChoiceData, Manager);
        }
    }
}

void UW_DialogBox::ShowHiddenButton()
{
    UE_LOG(LogTemp, Log, TEXT("히든 선택지 추가 함수가 호출 됨"));

    ALobbyGameModeBase* LobbyGameMode = GetLobbyGameMode();
    if (!VBox_Choices || !LobbyGameMode || !LobbyGameMode->ScenarioManagerComp)
    {
        return;
    }

    FScenarioData ChoiceData;
    if (LobbyGameMode->ScenarioManagerComp->GetSkipQuestion(ChoiceData))
    {
        AddChoiceButton(ChoiceData, LobbyGameMode->ScenarioManagerComp);
    }
}

FReply UW_DialogBox::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!bClickNext)
    {
        return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    }
    
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        ALobbyGameModeBase* LobbyGameMode = GetLobbyGameMode();
        if (LobbyGameMode && LobbyGameMode->ScenarioManagerComp)
        {
            LobbyGameMode->ScenarioManagerComp->RequestNextStep();
            return FReply::Handled();
        }
    }
    
    
    
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UW_DialogBox::NativeDestruct()
{
    GetWorld()->GetTimerManager().ClearTimer(DelayChoiceTimerHandler);
    Super::NativeDestruct();
}
