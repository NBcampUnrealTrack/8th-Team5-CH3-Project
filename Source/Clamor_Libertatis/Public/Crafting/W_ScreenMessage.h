// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_ScreenMessage.generated.h"


class UTextBlock;

/**
 * 
 */
UCLASS()
class UW_ScreenMessage : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageTextBlock;

public:
	void InitScreenMessage(FString Message);
};
