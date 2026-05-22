// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ScenarioData.generated.h"

UENUM(BlueprintType)
enum class ETalkType : uint8
{
    Text    UMETA(DisplayName = "Text"),
    Choice  UMETA(DisplayName = "Choice"),
    Question  UMETA(DisplayName = "Question"),
    End     UMETA(DisplayName = "End"),
};

UENUM(BlueprintType)
enum class EImageAction : uint8
{
    NoChange    UMETA(DisplayName = "NoChange"),
    Show        UMETA(DisplayName = "Show"),
    Hide        UMETA(DisplayName = "Hide")
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

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EImageAction ImageAction = EImageAction::NoChange;

    // ImageAction == Show일 때만 사용
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "ImageAction == EImageAction::Show"))
    TSoftObjectPtr<UTexture2D> Image;
};


