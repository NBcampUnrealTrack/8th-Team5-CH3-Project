// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ScenarioData.generated.h"

UENUM(BlueprintType)
enum class ETalkType : uint8
{
    Text    UMETA(DisplayName = "Text"),
    Choice  UMETA(DisplayName = "Choice")
};


USTRUCT(BlueprintType)
struct FScenarioData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ETalkType Type; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Dialogue; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName NextID; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FName> ChoiceIDs; 

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ConditionFlag;
};

// FTableRowBase 상속 없는 Blueprint 전용 선택지 데이터
USTRUCT(BlueprintType)
struct FScenarioChoiceEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FText Dialogue;

    UPROPERTY(BlueprintReadOnly)
    FName RowName;
};

USTRUCT(BlueprintType)
struct FScenarioChoiceList
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<FScenarioChoiceEntry> Choices;
};
