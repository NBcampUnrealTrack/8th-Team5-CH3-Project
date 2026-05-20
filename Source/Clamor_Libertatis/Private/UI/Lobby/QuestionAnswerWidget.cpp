#include "UI/Lobby/QuestionAnswerWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"

static constexpr float QTypingInterval = 0.04f;

void UQuestionAnswerWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);

    if (Text_Answer)   Text_Answer->SetText(FText::GetEmpty());
    if (Text_ClickHint) Text_ClickHint->SetVisibility(ESlateVisibility::Hidden);
}

void UQuestionAnswerWidget::ShowAnswer(const FText& InText)
{
    if (Anim_FadeIn) PlayAnimation(Anim_FadeIn);
    StartTyping(InText);
}

FReply UQuestionAnswerWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent)
{
    if (bIsTyping)
    {
        FinishTyping();
        return FReply::Handled();
    }

    if (bCanAdvance)
    {
        bCanAdvance = false;
        OnAnswerFinished.Broadcast();
    }

    return FReply::Handled();
}

void UQuestionAnswerWidget::StartTyping(const FText& InText)
{
    GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);

    FullText = InText.ToString();
    TypingIndex = 0;
    bIsTyping = true;
    bCanAdvance = false;

    if (Text_Answer)    Text_Answer->SetText(FText::GetEmpty());
    if (Text_ClickHint) Text_ClickHint->SetVisibility(ESlateVisibility::Hidden);

    GetWorld()->GetTimerManager().SetTimer(
        TypingTimerHandle, this,
        &UQuestionAnswerWidget::TickTyping,
        QTypingInterval, true
    );
}

void UQuestionAnswerWidget::TickTyping()
{
    if (TypingIndex >= FullText.Len())
    {
        FinishTyping();
        return;
    }
    if (Text_Answer)
        Text_Answer->SetText(
            FText::FromString(FullText.Left(++TypingIndex)));
}

void UQuestionAnswerWidget::FinishTyping()
{
    GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
    if (Text_Answer)    Text_Answer->SetText(FText::FromString(FullText));
    bIsTyping = false;
    bCanAdvance = true;
    if (Text_ClickHint) Text_ClickHint->SetVisibility(ESlateVisibility::Visible);
}