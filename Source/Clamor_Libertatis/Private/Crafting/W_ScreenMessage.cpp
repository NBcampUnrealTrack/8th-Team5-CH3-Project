// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/W_ScreenMessage.h"
#include "Components/TextBlock.h"

void UW_ScreenMessage::InitScreenMessage(FString Message)
{
    if (MessageTextBlock)
    {
        MessageTextBlock->SetText(FText::FromString(Message));
    }
}
