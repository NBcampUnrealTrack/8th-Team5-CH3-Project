// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndingModeBase.generated.h"

class UOpeningSequencer;
class UOpeningWidget;
class UAfterEndingWidget;

/**
 * 
 */
UCLASS()
class AEndingModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    AEndingModeBase();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Ending")
    TObjectPtr<UDataTable> EndingScenarioTable;

    UPROPERTY(EditDefaultsOnly, Category = "Ending|UI")
    TSubclassOf<UOpeningWidget> EndingWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Ending|UI")
    TSubclassOf<UAfterEndingWidget> AfterEndingWidgetClass;

private:
    UPROPERTY()
    TObjectPtr<UOpeningSequencer> EndingSequencer;

    UPROPERTY()
    TObjectPtr<UOpeningWidget> EndingWidget;

    UPROPERTY()
    TObjectPtr<UAfterEndingWidget> AfterEndingWidget;

    UFUNCTION()
    void OnEndingEnd();
};