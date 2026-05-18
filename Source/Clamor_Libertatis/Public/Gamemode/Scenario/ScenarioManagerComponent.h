// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScenarioData.h"
#include "ScenarioManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScenarioStepUpdated, 
    const FScenarioData&, MainData, 
    const TArray<FScenarioData>&, Choices
);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScenarioEnded);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UScenarioManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UScenarioManagerComponent();

    UFUNCTION(BlueprintCallable, Category = "Scenario")
    void StartScenario(FName RowName);

    UFUNCTION(BlueprintCallable, Category = "Scenario")
    void RequestNextStep();

    UPROPERTY(BlueprintAssignable, Category = "Scenario")
    FOnScenarioStepUpdated OnScenarioStepUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Scenario")
    FOnScenarioEnded OnScenarioEnded;

    UPROPERTY(EditAnywhere, Category = "Scenario|UI")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    UPROPERTY()
    UUserWidget* CachedDialogueWidget;

    bool IsStepAlreadyReadQuestion(FName TargetRowName) const;

    bool GetSkipQuestion(FScenarioData& Output) const;


private:
    bool SplitRowNameFromEnd(FName InRowName, FString& OutLeft, FString& OutRight) const;

    void UpdateDisplay();
    TArray<FScenarioData> BuildChoiceList(const FScenarioData& Data) const;

    void SaveLastRowName(FName RowName);


    UPROPERTY(EditAnywhere, Category = "Scenario")
    UDataTable* ScenarioTable;

    FName CurrentRowName;
};
