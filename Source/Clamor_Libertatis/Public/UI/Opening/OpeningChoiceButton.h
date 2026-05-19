#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpeningChoiceButton.generated.h"

class UButton;
class UTextBlock;
class UOpeningSequencer;

UCLASS()
class CLAMOR_LIBERTATIS_API UOpeningChoiceButton : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitButton(const FText& InLabel,
        const FName& InChoiceID,
        UOpeningSequencer* InSequencer);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Btn_Choice;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_Choice;

private:
    FName ChoiceID;

    UPROPERTY()
    TObjectPtr<UOpeningSequencer> Sequencer;

    UFUNCTION()
    void OnChoiceClicked();
};