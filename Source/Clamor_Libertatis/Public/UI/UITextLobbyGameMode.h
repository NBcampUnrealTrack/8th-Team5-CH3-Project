#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gamemode/Scenario/ScenarioData.h"
#include "UITextLobbyGameMode.generated.h"

class UScenarioManagerComponent;
class UQuestionWidget;
class UQuestionAnswerWidget;
class ULobbyWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API AUITextLobbyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AUITextLobbyGameMode();


    UPROPERTY(VisibleAnywhere, Category = "Scenario")
    TObjectPtr<UScenarioManagerComponent> ScenarioManagerComp;

    void ShowLobbyPhase();
    void ShowQuestionPhase(const TArray<FScenarioData>& Choices);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Question")
    TSubclassOf<UQuestionWidget> QuestionWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Question")
    TSubclassOf<UQuestionAnswerWidget> AnswerWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Lobby")
    TSubclassOf<ULobbyWidget> LobbyWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<UQuestionWidget> QuestionWidget;
    UPROPERTY()
    TObjectPtr<UQuestionAnswerWidget> AnswerWidget;
    UPROPERTY()
    TObjectPtr<ULobbyWidget> LobbyWidget;

    UFUNCTION()
    void OnQuestionSelected(FName NextID);
    UFUNCTION()
    void OnAnswerFinished();
    UFUNCTION()
    void HandleScenarioStepUpdated(
        const FScenarioData& MainData,
        const TArray<FScenarioData>& Choices);
    UFUNCTION()
    void HandleScenarioEnded();
    UFUNCTION()
    void OnStartBattleClicked();

    void ShowAnswerPhase(const FText& AnswerText);
    void HideAllWidgets();
    void GotoBattle();
    void SetMouseUI();
    void SetMouseGame();
};