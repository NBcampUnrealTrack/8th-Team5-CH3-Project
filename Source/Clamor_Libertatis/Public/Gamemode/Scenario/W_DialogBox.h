// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScenarioData.h"
#include "W_DialogBox.generated.h"

class UVerticalBox;
class UW_Button;
class ALobbyGameModeBase;
class UScenarioManagerComponent;

/**
 * 
 */
UCLASS()
class UW_DialogBox : public UUserWidget
{
	GENERATED_BODY()

public: 
    UW_DialogBox(const FObjectInitializer& ObjectInitializer);
	
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Txt_Dialog;

    UFUNCTION()
    void HandleScenarioEnded();

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VBox_Choices;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|UI")
    TSubclassOf<UW_Button> ButtonWidgetClass;


    UFUNCTION()
    void HandleScenarioStepUpdated(const FScenarioData& MainData, const TArray<FScenarioData>& Choices);

    void ShowHiddenButton();

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeDestruct() override;

private:
    bool bClickNext;
    FTimerHandle DelayChoiceTimerHandler;

    ALobbyGameModeBase* GetLobbyGameMode() const;
    void AddChoiceButton(const FScenarioData& ChoiceData, UScenarioManagerComponent* Manager);
    TArray<FScenarioData> GetRandomChoices(const TArray<FScenarioData>& AllChoices, int32 Count) const;
};
