#include "UI/UITestEndingGameMode.h"
#include "UI/Opening/OpeningSequencer.h"
#include "UI/Opening/OpeningWidget.h"
#include "UI/AfterEndingWidget.h"
#include "Gamemode/CLGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AUITestEndingGameMode::AUITestEndingGameMode()
{
}

void AUITestEndingGameMode::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();

    if (AfterEndingWidgetClass)
    {
        AfterEndingWidget = CreateWidget<UAfterEndingWidget>(
            GetWorld(), AfterEndingWidgetClass);
        if (AfterEndingWidget)
        {
            AfterEndingWidget->AddToViewport(10);
            AfterEndingWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (EndingWidgetClass)
    {
        EndingWidget = CreateWidget<UOpeningWidget>(
            GetWorld(), EndingWidgetClass);
        if (EndingWidget)
            EndingWidget->AddToViewport(5);
    }

    EndingSequencer = NewObject<UOpeningSequencer>(this);
    EndingSequencer->ScenarioTable = EndingScenarioTable;
    EndingSequencer->OnOpeningEnd.AddDynamic(
        this, &AUITestEndingGameMode::OnEndingEnd);

    if (EndingWidget)
        EndingWidget->InitWidget(EndingSequencer);

    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    FName StartRow = (GI && GI->bHasSelectedQuestion)
        ? FName("NormalEnding_1")// 한 번이라도 질문 선택
        : FName("TrueEnding_1");// 아무 질문 선택 안 함

    EndingSequencer->StartSequence(StartRow);

    if (PC)
    {
        PC->bShowMouseCursor = false;
        FInputModeUIOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

void AUITestEndingGameMode::OnEndingEnd()
{
    if (EndingWidget)
        EndingWidget->SetVisibility(ESlateVisibility::Collapsed);

    if (AfterEndingWidget)
    {
        AfterEndingWidget->SetVisibility(ESlateVisibility::Visible);

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->bShowMouseCursor = true;
            PC->bEnableClickEvents = true;
            FInputModeUIOnly InputMode;
            PC->SetInputMode(InputMode);
        }
    }
}