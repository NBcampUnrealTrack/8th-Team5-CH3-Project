#include "UI/QuickSlotWidget.h"
#include "UI/QuickSlotSlotWidget.h"
#include "Item/ConsumableInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "Engine/Texture2D.h"

void UQuickSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    QuickSlots.Init(NAME_None, 4);

    if (Slot1) Slot1->SlotIndex = 0;
    if (Slot2) Slot2->SlotIndex = 1;
    if (Slot3) Slot3->SlotIndex = 2;
    if (Slot4) Slot4->SlotIndex = 3;
}

void UQuickSlotWidget::InitQuickSlot(UConsumableInventoryComponent* InInventory)
{
    InventoryComp = InInventory;

    if (InventoryComp)
    {
        InventoryComp->OnInventoryChanged.AddDynamic(this, &UQuickSlotWidget::OnInventoryChanged);
    }

    RefreshAllSlots();
}

void UQuickSlotWidget::AssignToQuickSlot(int32 SlotIndex, FName ItemID)
{
    if (!QuickSlots.IsValidIndex(SlotIndex)) return;
    QuickSlots[SlotIndex] = ItemID;
    RefreshAllSlots();
}

FName UQuickSlotWidget::GetQuickSlotItem(int32 SlotIndex) const
{
    if (!QuickSlots.IsValidIndex(SlotIndex)) return NAME_None;
    return QuickSlots[SlotIndex];
}

void UQuickSlotWidget::UseQuickSlot(int32 SlotIndex)
{
    if (!QuickSlots.IsValidIndex(SlotIndex)) return;
    if (!InventoryComp) return;

    FName ItemID = QuickSlots[SlotIndex];
    if (ItemID.IsNone()) return;

    InventoryComp->UseItem(ItemID, 1);
}

void UQuickSlotWidget::RefreshAllSlots()
{
    RefreshSlot(0, Slot1);
    RefreshSlot(1, Slot2);
    RefreshSlot(2, Slot3);
    RefreshSlot(3, Slot4);
}

void UQuickSlotWidget::RefreshSlot(int32 SlotIndex, UQuickSlotSlotWidget* SlotWidget)
{
    if (!SlotWidget || !InventoryComp) return;

    FName ItemID = QuickSlots.IsValidIndex(SlotIndex) ? QuickSlots[SlotIndex] : NAME_None;
    if (ItemID.IsNone())
    {
        SlotWidget->ClearSlot();
        return;
    }

    int32 Quantity = InventoryComp->GetQuantityByID(ItemID);
    FItemTableRow* ItemData = InventoryComp->GetItemData(ItemID);

    UTexture2D* Icon = nullptr;
    if (ItemData && !ItemData->Icon.IsNull())
    {
        Icon = ItemData->Icon.LoadSynchronous();
    }

    SlotWidget->UpdateSlot(ItemID, Quantity, Icon);
}

void UQuickSlotWidget::OnInventoryChanged()
{
    RefreshAllSlots();
}