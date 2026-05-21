#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gamemode/Scenario/ScenarioData.h"
#include "QuestionChoiceButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionChoiceClicked, FName, NextID);

UCLASS()
class CLAMOR_LIBERTATIS_API UQuestionChoiceButton : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitButton(const FScenarioData& InData, bool bAlreadyRead);

    UPROPERTY(BlueprintAssignable)
    FOnQuestionChoiceClicked OnChoiceClicked;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    TObjectPtr<UTextBlock> Text_Choice;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Btn_Choice;

private:
    FName NextID;

    UFUNCTION()
    void OnClicked();
};