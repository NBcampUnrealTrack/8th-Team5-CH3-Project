#include "Crafting/W_CraftingSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "Item/ItemTableRow.h"
#include "Crafting/W_ItemTooltip.h"

void UW_CraftingSlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UW_CraftingSlot::HandleSlotClicked);
    }
}

void UW_CraftingSlot::Init(FName InItemID, int32 InAmount, UDataTable* ItemTable)
{
    CachedItemID = InItemID;

    FString ItemName = InItemID.ToString();
    const FItemTableRow* Row = ItemTable ? ItemTable->FindRow<FItemTableRow>(InItemID, TEXT("")) : nullptr;
    if (Row)
    {
        ItemName = Row->ItemName.ToString();
        ItemDescription = Row->Description;
    }
    else
    {
        ItemDescription = FText::GetEmpty();
    }

    if (ItemNameText) ItemNameText->SetText(FText::FromString(ItemName));
    if (AmountText)   AmountText->SetText(FText::AsNumber(InAmount));

    if (ItemIcon)
    {
        if (Row && !Row->Icon.IsNull())
        {
            UTexture2D* Texture = Row->Icon.LoadSynchronous();
            if (Texture)
            {
                ItemIcon->SetBrushFromTexture(Texture);
                ItemIcon->SetVisibility(ESlateVisibility::Visible);
            }
        }
        else
        {
            ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    
    if (ItemDetailToolTip)
    {
        UW_ItemTooltip* ToolTipInstance = CreateWidget<UW_ItemTooltip>(this, ItemDetailToolTip);
        if (ToolTipInstance)
        {
            ToolTipInstance->UpdateToolTip(ItemName, ItemDescription.ToString());
            SlotButton->SetToolTip(ToolTipInstance);
        }
    }
}

void UW_CraftingSlot::HandleSlotClicked()
{
    OnSlotClicked.Broadcast(CachedItemID);
}
