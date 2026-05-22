#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Gamemode/Scenario/ScenarioData.h"
#include "OpeningSequencer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRowReady, const FScenarioData&, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpeningEnd);

UCLASS(BlueprintType)
class CLAMOR_LIBERTATIS_API UOpeningSequencer : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
    UDataTable* ScenarioTable;

    UPROPERTY(BlueprintAssignable)
    FOnRowReady OnRowReady;

    UPROPERTY(BlueprintAssignable)
    FOnOpeningEnd OnOpeningEnd;

    void StartSequence(const FName& StartRowID = FName("Scenario_1"));

    UFUNCTION(BlueprintCallable, Category = "Opening")
    void Advance(const FName& NextRowID = NAME_None);

    UFUNCTION(BlueprintPure, Category = "Opening")
    bool IsRunning() const { return bRunning; }

private:
    bool bRunning = false;
    FName CurrentRowID = NAME_None;
    FTimerHandle PauseTimerHandle;

    void ShowRow(const FName& RowID);
    void EndSequence();
};