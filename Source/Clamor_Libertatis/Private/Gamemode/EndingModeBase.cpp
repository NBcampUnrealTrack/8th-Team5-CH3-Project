// Fill out your copyright notice in the Description page of Project Settings.

#include "Gamemode/EndingModeBase.h"
#include "UI/Opening/OpeningSequencer.h"
#include "UI/Opening/OpeningWidget.h"
#include "UI/AfterEndingWidget.h"
#include "Gamemode/CLGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AEndingModeBase::AEndingModeBase()
{
}

void AEndingModeBase::BeginPlay()
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
        {
            EndingWidget->AddToViewport(5);
        }
    }

    EndingSequencer = NewObject<UOpeningSequencer>(this);
    EndingSequencer->ScenarioTable = EndingScenarioTable;
    EndingSequencer->OnOpeningEnd.AddDynamic(
        this, &AEndingModeBase::OnEndingEnd);

    if (EndingWidget)
    {
        EndingWidget->InitWidget(EndingSequencer);
    }

    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    // 질문을 선택했을 때 노말 엔딩, 아무 선택 없을 때 트루 엔딩
    FName StartRow = (GI && GI->bHasSelectedQuestion)
        ? FName("NormalEnding_1")
        : FName("TrueEnding_1");

    EndingSequencer->StartSequence(StartRow);

    if (PC)
    {
        PC->bShowMouseCursor = false;
        FInputModeUIOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

void AEndingModeBase::OnEndingEnd()
{
    if (EndingWidget)
    {
        EndingWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

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
