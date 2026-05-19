#include "UI/Opening/OpeningSequencer.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UOpeningSequencer::StartSequence(const FName& StartRowID)
{
    if (!ScenarioTable)
    {
        UE_LOG(LogTemp, Error, TEXT("OpeningSequencer: ScenarioTable 없음"));
        EndSequence();
        return;
    }
    bRunning = true;
    ShowRow(StartRowID);
}

void UOpeningSequencer::Advance(const FName& NextRowID)
{
    if (!bRunning) return;

    GetWorld()->GetTimerManager().ClearTimer(PauseTimerHandle);

    FName TargetID = NextRowID;
    if (TargetID == NAME_None)
    {
        if (FScenarioData* Current = ScenarioTable->FindRow<FScenarioData>(
            CurrentRowID, TEXT("")))
        {
            TargetID = Current->NextID;
        }
    }

    if (TargetID == NAME_None || TargetID == FName("End"))
    {
        EndSequence();
        return;
    }

    ShowRow(TargetID);
}

void UOpeningSequencer::ShowRow(const FName& RowID)
{
    FScenarioData* Row = ScenarioTable->FindRow<FScenarioData>(
        RowID, TEXT("OpeningSequencer")
    );

    if (!Row)
    {
        UE_LOG(LogTemp, Warning,TEXT("OpeningSequencer: [%s] 없음 → 종료"), *RowID.ToString());
        EndSequence();
        return;
    }

    CurrentRowID = RowID;
    OnRowReady.Broadcast(*Row);
}

void UOpeningSequencer::EndSequence()
{
    bRunning = false;
    OnOpeningEnd.Broadcast();
}