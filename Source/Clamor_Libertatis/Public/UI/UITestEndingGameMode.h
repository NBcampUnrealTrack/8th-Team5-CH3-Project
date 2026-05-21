#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UITestEndingGameMode.generated.h"

class UOpeningSequencer;
class UOpeningWidget;
class UAfterEndingWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API AUITestEndingGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AUITestEndingGameMode();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Ending")
    TObjectPtr<UDataTable> EndingScenarioTable;

    UPROPERTY(EditDefaultsOnly, Category = "Ending|UI")
    TSubclassOf<UOpeningWidget> EndingWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Ending|UI")
    TSubclassOf<UAfterEndingWidget> AfterEndingWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<UOpeningSequencer> EndingSequencer;

    UPROPERTY()
    TObjectPtr<UOpeningWidget> EndingWidget;

    UPROPERTY()
    TObjectPtr<UAfterEndingWidget> AfterEndingWidget;

    UFUNCTION()
    void OnEndingEnd();
};