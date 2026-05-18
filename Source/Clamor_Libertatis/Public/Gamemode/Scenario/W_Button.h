// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Button.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceClicked, FName, NextID);

/**
 * 
 */
UCLASS()
class UW_Button : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FText ButtonText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    FName NextID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    bool bIsRead;



    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event")
    FOnChoiceClicked OnChoiceClicked;
};
