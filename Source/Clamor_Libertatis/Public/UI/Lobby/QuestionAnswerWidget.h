#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestionAnswerWidget.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnswerFinished);

UCLASS()
class CLAMOR_LIBERTATIS_API UQuestionAnswerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void ShowAnswer(const FText& InText);

    UPROPERTY(BlueprintAssignable)
    FOnAnswerFinished OnAnswerFinished;

protected:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_Answer;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_ClickHint;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> Anim_FadeIn;

private:
    FString  FullText;
    int32    TypingIndex = 0;
    FTimerHandle TypingTimerHandle;
    bool bIsTyping = false;
    bool bCanAdvance = false;

    void StartTyping(const FText& InText);
    void TickTyping();
    void FinishTyping();
};