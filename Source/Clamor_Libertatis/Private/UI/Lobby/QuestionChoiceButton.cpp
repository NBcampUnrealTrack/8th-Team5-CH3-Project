#include "UI/Lobby/QuestionChoiceButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UQuestionChoiceButton::InitButton(const FScenarioData& InData, bool bAlreadyRead)
{
    NextID = InData.NextID;

    if (Text_Choice)
        Text_Choice->SetText(InData.Dialogue);

    if (Btn_Choice)
    {
        // 이미 읽은 질문은 흐리게
        Btn_Choice->SetRenderOpacity(bAlreadyRead ? 0.35f : 1.0f);
        Btn_Choice->OnClicked.AddDynamic(this, &UQuestionChoiceButton::OnClicked);
    }
}

void UQuestionChoiceButton::OnClicked()
{
    OnChoiceClicked.Broadcast(NextID);
}