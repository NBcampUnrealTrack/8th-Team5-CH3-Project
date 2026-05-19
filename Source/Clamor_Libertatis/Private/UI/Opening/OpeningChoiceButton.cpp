#include "UI/Opening/OpeningChoiceButton.h"
#include "UI/Opening/OpeningSequencer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UOpeningChoiceButton::InitButton(const FText& InLabel,
    const FName& InChoiceID,
    UOpeningSequencer* InSequencer)
{
    ChoiceID = InChoiceID;
    Sequencer = InSequencer;

    if (Text_Choice)
        Text_Choice->SetText(InLabel);

    if (Btn_Choice)
        Btn_Choice->OnClicked.AddDynamic(
            this, &UOpeningChoiceButton::OnChoiceClicked);
}

void UOpeningChoiceButton::OnChoiceClicked()
{
    if (Sequencer) Sequencer->Advance(ChoiceID);
}