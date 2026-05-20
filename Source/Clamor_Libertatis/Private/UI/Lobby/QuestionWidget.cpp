#include "UI/Lobby/QuestionWidget.h"
#include "UI/Lobby/QuestionChoiceButton.h"
#include "Gamemode/Scenario/ScenarioManagerComponent.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void UQuestionWidget::InitWidget(UScenarioManagerComponent* InManager)
{
    if (!InManager) return;
    Manager = InManager;
}

void UQuestionWidget::ShowChoices(const TArray<FScenarioData>& Choices)
{
    if (!Box_Choices || !ChoiceButtonClass) return;

    Box_Choices->ClearChildren();

    TArray<FScenarioData> RandomChoices = GetRandomChoices(Choices, 3);

    for (const FScenarioData& Data : RandomChoices)
    {
        UQuestionChoiceButton* Button =
            CreateWidget<UQuestionChoiceButton>(this, ChoiceButtonClass);
        if (!Button) continue;

  
        bool bRead = Manager ? Manager->IsStepAlreadyReadQuestion(Data.NextID) : false;

        Button->InitButton(Data, bRead);
        Button->OnChoiceClicked.AddDynamic(this, &UQuestionWidget::HandleChoiceClicked);
        Box_Choices->AddChild(Button);
    }
}

void UQuestionWidget::HandleChoiceClicked(FName NextID)
{
    OnQuestionSelected.Broadcast(NextID);
}

TArray<FScenarioData> UQuestionWidget::GetRandomChoices(
    const TArray<FScenarioData>& All, int32 Count) const
{
    TArray<FScenarioData> Shuffled = All;
    for (int32 i = Shuffled.Num() - 1; i > 0; i--)
        Shuffled.Swap(i, FMath::RandRange(0, i));

    Shuffled.SetNum(FMath::Min(Count, Shuffled.Num()));
    return Shuffled;
}