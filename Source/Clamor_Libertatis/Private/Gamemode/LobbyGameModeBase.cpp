// Fill out your copyright notice in the Description page of Project Settings.

#include "Gamemode/LobbyGameModeBase.h"
#include "Gamemode/CLGameInstance.h"
#include "Gamemode/Scenario/ScenarioManagerComponent.h"
#include "UI/Lobby/QuestionWidget.h"
#include "UI/Lobby/QuestionAnswerWidget.h"
#include "UI/Lobby/LobbyWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


ALobbyGameModeBase::ALobbyGameModeBase()
{
    ScenarioManagerComp = CreateDefaultSubobject<UScenarioManagerComponent>(TEXT("ScenarioManagerComp"));
}

void ALobbyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (QuestionWidgetClass)
    {
        QuestionWidget = CreateWidget<UQuestionWidget>(GetWorld(), QuestionWidgetClass);
        if (QuestionWidget)
        {
            QuestionWidget->AddToViewport(10);
            QuestionWidget->SetVisibility(ESlateVisibility::Collapsed);
            QuestionWidget->InitWidget(ScenarioManagerComp);
            QuestionWidget->OnQuestionSelected.AddDynamic(this, &ALobbyGameModeBase::OnQuestionSelected);
        }
    }

    if (AnswerWidgetClass)
    {
        AnswerWidget = CreateWidget<UQuestionAnswerWidget>(GetWorld(), AnswerWidgetClass);
        if (AnswerWidget)
        {
            AnswerWidget->AddToViewport(10);
            AnswerWidget->SetVisibility(ESlateVisibility::Collapsed);
            AnswerWidget->OnAnswerFinished.AddDynamic(this, &ALobbyGameModeBase::OnAnswerFinished);
        }
    }

    if (LobbyWidgetClass)
    {
        LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidgetClass);
        if (LobbyWidget)
        {
            LobbyWidget->AddToViewport(10);
            LobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
            LobbyWidget->OnStartBattleClicked.AddDynamic(this, &ALobbyGameModeBase::GotoBattle);
        }
    }

    if (ScenarioManagerComp)
    {
        ScenarioManagerComp->OnScenarioStepUpdated.AddDynamic(this, &ALobbyGameModeBase::HandleScenarioStepUpdated);
        ScenarioManagerComp->OnScenarioEnded.AddDynamic(this, &ALobbyGameModeBase::ShowLobbyPhase);
    }

    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    if (GI)
    {
        SetMouseUI();

        if (GI->IsGameOver()) 
        {
            ShowLobbyPhase();
        }
        else if (GI->HasWatchedOpening() && ScenarioManagerComp)
        {                        
            ScenarioManagerComp->StartScenario("Question_0");            
        }
     }
}


void ALobbyGameModeBase::HideAllWidgets()
{
    if (QuestionWidget)
    {
        QuestionWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (AnswerWidget)
    {
        AnswerWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (LobbyWidget)
    {
        LobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ALobbyGameModeBase::ShowQuestionPhase(const TArray<FScenarioData>& Choices)
{
    HideAllWidgets();
    SetMouseUI();
    if (QuestionWidget)
    {
        QuestionWidget->SetVisibility(ESlateVisibility::Visible);
        QuestionWidget->ShowChoices(Choices);
    }
}

void ALobbyGameModeBase::ShowAnswerPhase(const FText& AnswerText)
{
    HideAllWidgets();
    if (AnswerWidget)
    {
        AnswerWidget->SetVisibility(ESlateVisibility::Visible);
        AnswerWidget->ShowAnswer(AnswerText);
    }
}

void ALobbyGameModeBase::ShowLobbyPhase()
{
    HideAllWidgets();
    SetMouseUI();
    if (LobbyWidget)
    {
        UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
        int32 Count = GI ? GI->GetWonBattleCount() : 0;
        LobbyWidget->SetVisibility(ESlateVisibility::Visible);
        LobbyWidget->InitLobby(Count);
    }
}

void ALobbyGameModeBase::HandleScenarioStepUpdated(const FScenarioData& MainData, const TArray<FScenarioData>& Choices)
{
    if (MainData.Type == ETalkType::Question)
    {
        ShowQuestionPhase(Choices);
    }
    else
    {
        ShowAnswerPhase(MainData.Dialogue);
    }
}

void ALobbyGameModeBase::OnQuestionSelected(FName NextID)
{
    if (NextID == FName("SkipQuestion"))
    {
        ShowLobbyPhase();
        return;
    }
    if (ScenarioManagerComp)
    {
        ScenarioManagerComp->StartScenario(NextID);
    }
}

void ALobbyGameModeBase::OnAnswerFinished()
{
    if (ScenarioManagerComp)
    {
        ScenarioManagerComp->RequestNextStep();
    }
}

void ALobbyGameModeBase::ReadyComplete()
{
    if (!ScenarioManagerComp->IsScenarioEnd())
        return;

    UE_LOG(LogTemp, Warning, TEXT("Called Ready Complete"));
    GotoBattle();
}

void ALobbyGameModeBase::OpenCraftingWindow()
{
    if (CraftingWindowClass)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            UUserWidget* CraftingWindow = CreateWidget<UUserWidget>(PC, CraftingWindowClass);
            if (CraftingWindow)
            {
                CraftingWindow->AddToViewport(999);
            }
        }
    }
}

void ALobbyGameModeBase::GotoBattle()
{
    HideAllWidgets();
    SetMouseGame();

    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    if (!GI)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Lobby] Failed to get GameInstance"));
        return;
    }

    int32 Count = GI->GetWonBattleCount();
    UE_LOG(LogTemp, Warning, TEXT("[Lobby] Current Won Battle Count: %d"), Count);


    if (Count >= 3)
    {
        GI->ResetGame();
        UGameplayStatics::OpenLevel(this, FName("/Game/UI/L_UITestMap"));
        return;
    }

    FString Level =
        Count == 0 ? TEXT("/Game/Level/L_Stage1") :
        Count == 1 ? TEXT("/Game/Level/L_Stage2") :
                     TEXT("/Game/Level/L_Stage3");

    UGameplayStatics::OpenLevel(this, FName(*Level));
}

void ALobbyGameModeBase::SetMouseUI()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;
    PC->bShowMouseCursor = true;
    PC->bEnableClickEvents = true;
    PC->bEnableMouseOverEvents = true;
    FInputModeUIOnly InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);
}

void ALobbyGameModeBase::SetMouseGame()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;
    PC->bShowMouseCursor = false;
    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);
}
