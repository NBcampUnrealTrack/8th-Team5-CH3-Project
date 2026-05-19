#include "UI/Opening/OpeningWidget.h"
#include "UI/Opening/OpeningSequencer.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "TimerManager.h"
#include "Engine/World.h"

static constexpr float TypingInterval = 0.04f;

void UOpeningWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Text_Dialogue)  Text_Dialogue->SetText(FText::GetEmpty());
    if (Text_ClickHint) Text_ClickHint->SetVisibility(ESlateVisibility::Hidden);
    if (Box_Choices)    Box_Choices->SetVisibility(ESlateVisibility::Collapsed);
    if (Text_Counter)   Text_Counter->SetText(FText::GetEmpty());
}

void UOpeningWidget::InitWidget(UOpeningSequencer* InSequencer)
{
    UE_LOG(LogTemp, Warning, TEXT("InitWidget 호출됨"));

    if (!InSequencer)
    {
        UE_LOG(LogTemp, Error, TEXT("InSequencer 없음"));
        return;
    }

    Sequencer = InSequencer;

    if (Sequencer->ScenarioTable)
        TotalCount = Sequencer->ScenarioTable->GetRowNames().Num();

    UE_LOG(LogTemp, Warning, TEXT("OnRowReady 바인딩 시도"));
    Sequencer->OnRowReady.AddDynamic(this, &UOpeningWidget::HandleRowReady);
    UE_LOG(LogTemp, Warning, TEXT("OnRowReady 바인딩 완료"));
}

FReply UOpeningWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent)
{
    if (bIsTyping)
    {
        FinishTyping();
        return FReply::Handled();
    }

    if (bCanAdvance && Sequencer)
    {
        bCanAdvance = false;
        if (Text_ClickHint)
            Text_ClickHint->SetVisibility(ESlateVisibility::Hidden);
        Sequencer->Advance();
    }

    return FReply::Handled();
}

void UOpeningWidget::HandleRowReady(const FScenarioData& Row)
{
    UE_LOG(LogTemp, Warning, TEXT("HandleRowReady 호출됨 - Type: %d"), (int32)Row.Type);
    CurrentIndex++;
    UpdateCounter();
    HandleImageAction(Row);

    if (Row.Type == ETalkType::Choice)
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
        bIsTyping = false;
        bCanAdvance = false;

        if (Text_Dialogue)
            Text_Dialogue->SetText(FText::GetEmpty());
        if (Text_ClickHint)
            Text_ClickHint->SetVisibility(ESlateVisibility::Hidden);

        BuildChoiceButtons(Row.ChoiceIDs);
        return;
    }

    if (Row.Type == ETalkType::End)
    {
        if (Sequencer) Sequencer->Advance(FName("End"));
        return;
    }

    if (Box_Choices)
    {
        Box_Choices->ClearChildren();
        Box_Choices->SetVisibility(ESlateVisibility::Collapsed);
    }
    StartTyping(Row.Dialogue);
}

void UOpeningWidget::StartTyping(const FText& InText)
{
    GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);

    FullText = InText.ToString();
    TypingIndex = 0;
    bIsTyping = true;
    bCanAdvance = false;

    if (Text_Dialogue)
        Text_Dialogue->SetText(FText::GetEmpty());
    if (Text_ClickHint)
        Text_ClickHint->SetVisibility(ESlateVisibility::Hidden);

    GetWorld()->GetTimerManager().SetTimer(
        TypingTimerHandle,
        this,
        &UOpeningWidget::TickTyping,
        TypingInterval,
        true
    );
}

void UOpeningWidget::TickTyping()
{
    if (TypingIndex >= FullText.Len())
    {
        FinishTyping();
        return;
    }

    if (Text_Dialogue)
        Text_Dialogue->SetText(
            FText::FromString(FullText.Left(++TypingIndex)));
}

void UOpeningWidget::FinishTyping()
{
    GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);

    if (Text_Dialogue)
        Text_Dialogue->SetText(FText::FromString(FullText));

    bIsTyping = false;
    bCanAdvance = true;

    if (Text_ClickHint)
        Text_ClickHint->SetVisibility(ESlateVisibility::Visible);
}

void UOpeningWidget::BuildChoiceButtons(const TArray<FName>& ChoiceIDs)
{
    UE_LOG(LogTemp, Warning, TEXT("BuildChoiceButtons 호출 - ChoiceIDs 수: %d"), ChoiceIDs.Num());

    if (!Box_Choices) { UE_LOG(LogTemp, Error, TEXT("Box_Choices 없음")); return; }
    if (!ChoiceButtonClass) { UE_LOG(LogTemp, Error, TEXT("ChoiceButtonClass 없음")); return; }
    if (!Sequencer) { UE_LOG(LogTemp, Error, TEXT("Sequencer 없음")); return; }

    Box_Choices->ClearChildren();
    Box_Choices->SetVisibility(ESlateVisibility::Visible);

    for (const FName& ChoiceID : ChoiceIDs)
    {
        UE_LOG(LogTemp, Warning, TEXT("버튼 생성 시도: %s"), *ChoiceID.ToString());

        FScenarioData* Row = Sequencer->ScenarioTable->FindRow<FScenarioData>(
            ChoiceID, TEXT("")
        );

        if (!Row) { UE_LOG(LogTemp, Error, TEXT("Row 없음: %s"), *ChoiceID.ToString()); continue; }

        UOpeningChoiceButton* Button =
            CreateWidget<UOpeningChoiceButton>(this, ChoiceButtonClass);

        if (!Button) { UE_LOG(LogTemp, Error, TEXT("Button 생성 실패")); continue; }

        Button->InitButton(Row->Dialogue, ChoiceID, Sequencer);
        Box_Choices->AddChild(Button);

        UE_LOG(LogTemp, Warning, TEXT("버튼 추가 완료: %s"), *ChoiceID.ToString());
    }
}

void UOpeningWidget::HandleImageAction(const FScenarioData& Row)
{
    if (!Image_Background) return;

    switch (Row.ImageAction)
    {
    case EImageAction::Show:
        if (!Row.Image.IsNull())
        {
            Row.Image.LoadSynchronous();
            if (UTexture2D* Tex = Row.Image.Get())
                Image_Background->SetBrushFromTexture(Tex);
        }
        if (Anim_FadeIn) PlayAnimation(Anim_FadeIn);
        break;

    case EImageAction::Hide:
        if (Anim_FadeOut) PlayAnimation(Anim_FadeOut);
        break;

    case EImageAction::NoChange:
    default:
        break;
    }
}

void UOpeningWidget::UpdateCounter()
{
    if (!Text_Counter) return;
    Text_Counter->SetText(FText::FromString(
        FString::Printf(TEXT("씬 %d / %d"), CurrentIndex, TotalCount)));
}