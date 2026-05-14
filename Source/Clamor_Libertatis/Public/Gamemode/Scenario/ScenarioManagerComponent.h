// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScenarioData.h"
#include "ScenarioManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScenarioStepUpdated, FScenarioData, MainData);

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

    // 이벤트 발생 후 Blueprint에서 호출해서 선택지 가져오기
    UFUNCTION(BlueprintPure, Category = "Scenario")
    FScenarioChoiceList GetCurrentChoices() const
    {
        UE_LOG(LogTemp, Warning, TEXT("[ScenarioManager] GetCurrentChoices 호출 - 인스턴스: %s, 개수: %d"), *GetOwner()->GetName(), CachedChoiceList.Choices.Num());
        return CachedChoiceList;
    }

    // 다음 데이터 표시 - UI 업데이트
    UPROPERTY(BlueprintAssignable, Category = "Scenario")
    FOnScenarioStepUpdated OnScenarioStepUpdated;

    // 시나리오 종료 - UI 닫기
    UPROPERTY(BlueprintAssignable, Category = "Scenario")
    FOnScenarioEnded OnScenarioEnded;

    UPROPERTY(EditAnywhere, Category = "Scenario|UI")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    // 생성된 위젯을 참조 (중복 생성 방지 및 관리용)
    UPROPERTY()
    UUserWidget* CachedDialogueWidget;


private:
    void UpdateDisplay();
    FScenarioChoiceList BuildChoiceList(const FScenarioData& Data) const;

    UPROPERTY(EditAnywhere, Category = "Scenario")
    UDataTable* ScenarioTable;

    FName CurrentRowName;

    UPROPERTY()
    FScenarioChoiceList CachedChoiceList;
};
