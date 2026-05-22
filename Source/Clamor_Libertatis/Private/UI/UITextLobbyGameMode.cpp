#include "UI/UITextLobbyGameMode.h"
#include "Gamemode/Scenario/ScenarioManagerComponent.h"
#include "Gamemode/CLGameInstance.h"
#include "UI/Lobby/QuestionWidget.h"
#include "UI/Lobby/QuestionAnswerWidget.h"
#include "UI/Lobby/LobbyWidget.h"
#include "Character/BasePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AUITextLobbyGameMode::AUITextLobbyGameMode()
{
    ScenarioManagerComp = CreateDefaultSubobject<UScenarioManagerComponent>(
        TEXT("ScenarioManagerComp"));
}

void AUITextLobbyGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT(" UITextLobbyGameMode BeginPlay"));

    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();

    UE_LOG(LogTemp, Warning, TEXT("HasWatchedOpening: %s"),
        GI && GI->HasWatchedOpening() ? TEXT("true") : TEXT("false"));
    // 위젯 생성
    if (QuestionWidgetClass)
    {
        QuestionWidget = CreateWidget<UQuestionWidget>(
            GetWorld(), QuestionWidgetClass);
        if (QuestionWidget)
        {
            QuestionWidget->AddToViewport(10);
            QuestionWidget->SetVisibility(ESlateVisibility::Collapsed);
            QuestionWidget->InitWidget(ScenarioManagerComp);
            QuestionWidget->OnQuestionSelected.AddDynamic(
                this, &AUITextLobbyGameMode::OnQuestionSelected);
        }
    }

    if (AnswerWidgetClass)
    {
        AnswerWidget = CreateWidget<UQuestionAnswerWidget>(
            GetWorld(), AnswerWidgetClass);
        if (AnswerWidget)
        {
            AnswerWidget->AddToViewport(10);
            AnswerWidget->SetVisibility(ESlateVisibility::Collapsed);
            AnswerWidget->OnAnswerFinished.AddDynamic(
                this, &AUITextLobbyGameMode::OnAnswerFinished);
        }
    }

    if (LobbyWidgetClass)
    {
        LobbyWidget = CreateWidget<ULobbyWidget>(
            GetWorld(), LobbyWidgetClass);
        if (LobbyWidget)
        {
            LobbyWidget->AddToViewport(10);
            LobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
            LobbyWidget->OnStartBattleClicked.AddDynamic(
                this, &AUITextLobbyGameMode::OnStartBattleClicked);
        }
    }

    // ScenarioManager 바인딩
    if (ScenarioManagerComp)
    {
        ScenarioManagerComp->OnScenarioStepUpdated.AddDynamic(
            this, &AUITextLobbyGameMode::HandleScenarioStepUpdated);
        ScenarioManagerComp->OnScenarioEnded.AddDynamic(
            this, &AUITextLobbyGameMode::HandleScenarioEnded);
    }

    // 최초 실행 -> GameStart UI (BasePlayerController가 처리)
    // 이후 로비 진입 -> 바로 질문 시작
    if (GI && GI->HasWatchedOpening())
    {
        UE_LOG(LogTemp, Warning, TEXT("질문 시작 시도"));
        SetMouseUI();
        if (ScenarioManagerComp)
        {
            UE_LOG(LogTemp, Warning, TEXT("StartScenario Question_0"));
            ScenarioManagerComp->StartScenario("Question_0");
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ScenarioManagerComp 없음"));
        }
    }
    // 최초 실행은 BasePlayerController::BeginPlay에서
    // ShowGameStartUI ->  Start 버튼 -> 오프닝 -> OnOpeningEnd -> 질문 시작
}

void AUITextLobbyGameMode::HideAllWidgets()
{
    if (QuestionWidget) QuestionWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (AnswerWidget)   AnswerWidget->SetVisibility(ESlateVisibility::Collapsed);
    if (LobbyWidget)    LobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AUITextLobbyGameMode::HandleScenarioStepUpdated(
    const FScenarioData& MainData,
    const TArray<FScenarioData>& Choices)
{
    if (MainData.Type == ETalkType::Question)
        ShowQuestionPhase(Choices);
    else
        ShowAnswerPhase(MainData.Dialogue);
}

void AUITextLobbyGameMode::ShowQuestionPhase(const TArray<FScenarioData>& Choices)
{
    HideAllWidgets();
    SetMouseUI();
    if (QuestionWidget)
    {
        QuestionWidget->SetVisibility(ESlateVisibility::Visible);
        QuestionWidget->ShowChoices(Choices);
    }
}

void AUITextLobbyGameMode::ShowAnswerPhase(const FText& AnswerText)
{
    HideAllWidgets();
    if (AnswerWidget)
    {
        AnswerWidget->SetVisibility(ESlateVisibility::Visible);
        AnswerWidget->ShowAnswer(AnswerText);
    }
}

void AUITextLobbyGameMode::ShowLobbyPhase()
{
    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();

    if (GI && GI->GetWonBattleCount() >= 3)
        return;

    HideAllWidgets();
    SetMouseUI();
    if (LobbyWidget)
    {
        int32 Count = GI ? GI->GetWonBattleCount() : 0;
        LobbyWidget->SetVisibility(ESlateVisibility::Visible);
        LobbyWidget->InitLobby(Count);
    }
}

void AUITextLobbyGameMode::OnQuestionSelected(FName NextID)
{
    if (NextID == FName("SkipQuestion"))
    {
        UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();

        // 3스테이지 클리어 후 스킵 -> 바로 엔딩
        if (GI && GI->GetWonBattleCount() >= 3)
        {
            HideAllWidgets();
            SetMouseGame();
            UGameplayStatics::OpenLevel(this, FName("/Game/UI/L_UITestEndingMap"));
            return;
        }

        // 나머지 -> 로비 UI
        ShowLobbyPhase();
        return;
    }

    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
        GI->bHasSelectedQuestion = true;

    if (ScenarioManagerComp)
        ScenarioManagerComp->StartScenario(NextID);
}

void AUITextLobbyGameMode::OnAnswerFinished()
{
    if (ScenarioManagerComp)
        ScenarioManagerComp->RequestNextStep();
}

void AUITextLobbyGameMode::HandleScenarioEnded()
{
    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();

    UE_LOG(LogTemp, Warning, TEXT("HandleScenarioEnded - WonBattleCount: %d"),
        GI ? GI->GetWonBattleCount() : -1);

    if (GI && GI->GetWonBattleCount() >= 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("엔딩으로 이동"));
        HideAllWidgets();
        SetMouseGame();
        UGameplayStatics::OpenLevel(
            this, FName("/Game/UI/L_UITestEndingMap"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("로비 UI 표시"));
    ShowLobbyPhase();
}
void AUITextLobbyGameMode::OnStartBattleClicked()
{
    GotoBattle();
}

void AUITextLobbyGameMode::GotoBattle()
{
    HideAllWidgets();
    SetMouseGame();

    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    if (!GI) return;

    int32 Count = GI->GetWonBattleCount();

    if (Count >= 3)
    {
        UGameplayStatics::OpenLevel(
            this, FName("/Game/UI/L_UITestEndingMap")); //엔딩 레벨로
        return;
    }
    FString Level =
        Count == 0 ? TEXT("/Game/Level/L_Stage1") :
        Count == 1 ? TEXT("/Game/Level/L_Stage2") :
        TEXT("/Game/Level/L_Stage3");

    UGameplayStatics::OpenLevel(this, FName(*Level));
}

void AUITextLobbyGameMode::SetMouseUI()
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

void AUITextLobbyGameMode::SetMouseGame()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;
    PC->bShowMouseCursor = false;
    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);
}