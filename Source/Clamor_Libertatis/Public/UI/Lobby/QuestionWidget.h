#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gamemode/Scenario/ScenarioData.h"
#include "UI/Lobby/QuestionChoiceButton.h"
#include "QuestionWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UScenarioManagerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionSelected, FName, NextID);

UCLASS()
class CLAMOR_LIBERTATIS_API UQuestionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitWidget(UScenarioManagerComponent* InManager);
    void ShowChoices(const TArray<FScenarioData>& Choices);

    UPROPERTY(BlueprintAssignable)
    FOnQuestionSelected OnQuestionSelected;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> Box_Choices;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_Guide; 

    UPROPERTY(EditDefaultsOnly, Category = "Question")
    TSubclassOf<UQuestionChoiceButton> ChoiceButtonClass;

private:
    UPROPERTY()
    TObjectPtr<UScenarioManagerComponent> Manager;

    UFUNCTION()
    void HandleChoiceClicked(FName NextID);

    TArray<FScenarioData> GetRandomChoices(
        const TArray<FScenarioData>& All, int32 Count) const;
};