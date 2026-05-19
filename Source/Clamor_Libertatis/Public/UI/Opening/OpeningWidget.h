#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gamemode/Scenario/ScenarioData.h"
#include "UI/Opening/OpeningChoiceButton.h"
#include "OpeningWidget.generated.h"

class UOpeningSequencer;
class UTextBlock;
class UImage;
class UVerticalBox;

UCLASS()
class CLAMOR_LIBERTATIS_API UOpeningWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitWidget(UOpeningSequencer* InSequencer);

protected:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(
        const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_Dialogue;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Image_Background;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> Box_Choices;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_ClickHint;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_Counter;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> Anim_FadeIn;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> Anim_FadeOut;

    UPROPERTY(EditDefaultsOnly, Category = "Opening")
    TSubclassOf<UOpeningChoiceButton> ChoiceButtonClass;

private:
    UPROPERTY()
    TObjectPtr<UOpeningSequencer> Sequencer;

    int32 CurrentIndex = 0;
    int32 TotalCount = 0;

    FString  FullText;
    int32    TypingIndex = 0;
    FTimerHandle TypingTimerHandle;
    bool bIsTyping = false;
    bool bCanAdvance = false;

    UFUNCTION()
    void HandleRowReady(const FScenarioData& Row);

    void StartTyping(const FText& InText);
    void TickTyping();
    void FinishTyping();

    void BuildChoiceButtons(const TArray<FName>& ChoiceIDs);
    void HandleImageAction(const FScenarioData& Row);
    void UpdateCounter();
};