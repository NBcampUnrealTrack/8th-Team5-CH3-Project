#include "UI/Inventory/QuickSlotWidget.h"
#include "UI/Inventory/QuickSlotSlotWidget.h"
#include "Item/Inventory/ConsumableInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "Engine/Texture2D.h"

void UQuickSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Slot1) { Slot1->SlotIndex = 0; Slot1->InitQuickSlotSlot(this); Slot1->SetKeyText("1"); }
    if (Slot2) { Slot2->SlotIndex = 1; Slot2->InitQuickSlotSlot(this); Slot2->SetKeyText("2"); }
    if (Slot3) { Slot3->SlotIndex = 2; Slot3->InitQuickSlotSlot(this); Slot3->SetKeyText("3"); }
    if (Slot4) { Slot4->SlotIndex = 3; Slot4->InitQuickSlotSlot(this); Slot4->SetKeyText("4"); }
}

void UQuickSlotWidget::InitQuickSlot(UConsumableInventoryComponent* InInventory)
{
    InventoryComp = InInventory;

    if (InventoryComp)
    {
        InventoryComp->OnInventoryChanged.AddDynamic(
            this, &UQuickSlotWidget::OnInventoryChanged);
    }

    //SetVisibility(ESlateVisibility::Visible);
    RefreshAllSlots();
}

void UQuickSlotWidget::AssignToQuickSlot(int32 SlotIndex, FName ItemID)
{
    RefreshAllSlots();
    int32 SourceIndex = INDEX_NONE;
    for (int32 i = 0; i < InventoryComp->Slots.Num(); i++)
    {
        if (InventoryComp->Slots[i].ItemID == ItemID)
        {
            SourceIndex = i;
            break;
        }
    }
    if (SourceIndex == INDEX_NONE || SourceIndex == SlotIndex) return;

    InventoryComp->Slots.Swap(SourceIndex, SlotIndex);
    InventoryComp->OnInventoryChanged.Broadcast();
}

void UQuickSlotWidget::UseQuickSlot(int32 SlotIndex)
{
    if (!InventoryComp) return;

    if (!InventoryComp->Slots.IsValidIndex(SlotIndex)) return;

    const FInventorySlot& InvSlot = InventoryComp->Slots[SlotIndex];
    if (InvSlot.IsEmpty()) return;

    InventoryComp->UseItem(InvSlot.ItemID, 1);
}

void UQuickSlotWidget::RefreshAllSlots()
{
    RefreshSlot(0, Slot1);
    RefreshSlot(1, Slot2);
    RefreshSlot(2, Slot3);
    RefreshSlot(3, Slot4);

    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UQuickSlotWidget::SwapQuickSlots(int32 FromIndex, int32 ToIndex)
{
    if (!InventoryComp) return;
    if (FromIndex == ToIndex) return;
    if (!InventoryComp->Slots.IsValidIndex(FromIndex)) return;
    if (!InventoryComp->Slots.IsValidIndex(ToIndex)) return;

    InventoryComp->Slots.Swap(FromIndex, ToIndex);
    InventoryComp->OnInventoryChanged.Broadcast();
}

void UQuickSlotWidget::RefreshSlot(int32 InventorySlotIndex, UQuickSlotSlotWidget* SlotWidget)
{
    if (!SlotWidget || !InventoryComp) return;

    if (!InventoryComp->Slots.IsValidIndex(InventorySlotIndex)
        || InventoryComp->Slots[InventorySlotIndex].IsEmpty())
    {
        SlotWidget->ClearSlot();
        return;
    }

    const FInventorySlot& InvSlot = InventoryComp->Slots[InventorySlotIndex];

    UTexture2D* Icon = nullptr;
    FItemTableRow* ItemData = InventoryComp->GetItemData(InvSlot.ItemID);
    if (ItemData && !ItemData->Icon.IsNull())
    {
        Icon = ItemData->Icon.LoadSynchronous();
    }

    SlotWidget->UpdateSlot(InvSlot.ItemID, InvSlot.Quantity, Icon);
}

void UQuickSlotWidget::OnInventoryChanged()
{
    RefreshAllSlots();
}