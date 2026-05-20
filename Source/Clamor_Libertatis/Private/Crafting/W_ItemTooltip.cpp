// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/W_ItemTooltip.h"
#include "Components/TextBlock.h"

void UW_ItemTooltip::UpdateToolTip(FString ItemName, FString Description)
{
    
    if (ItemNameText)
    {
        ItemNameText->SetText(FText::FromString(ItemName));
    }

    
    if (ItemDescriptionText)
    {
        ItemDescriptionText->SetText(FText::FromString(Description));
    }
}
