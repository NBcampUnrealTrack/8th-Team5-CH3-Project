#include "UI/InventoryWidget.h"
#include "UI/InventorySlotWidget.h"
#include "UI/QuickSlotWidget.h"
#include "UI/QuickSlotSlotWidget.h"
#include "Item/ConsumableInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "Components/WrapBox.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventoryWidget::InitInventory(UConsumableInventoryComponent* InInventory,
    UQuickSlotWidget* InQuickSlotWidget)
{
    InventoryComp = InInventory;
    QuickSlotWidgetRef = InQuickSlotWidget;

    if (QuickSlotWidgetRef)
    {
        if (QuickSlot1) { QuickSlot1->SlotIndex = 0; QuickSlot1->InitQuickSlotSlot(QuickSlotWidgetRef); }
        if (QuickSlot2) { QuickSlot2->SlotIndex = 1; QuickSlot2->InitQuickSlotSlot(QuickSlotWidgetRef); }
        if (QuickSlot3) { QuickSlot3->SlotIndex = 2; QuickSlot3->InitQuickSlotSlot(QuickSlotWidgetRef); }
        if (QuickSlot4) { QuickSlot4->SlotIndex = 3; QuickSlot4->InitQuickSlotSlot(QuickSlotWidgetRef); }
    }

    if (InventoryComp)
    {
        InventoryComp->OnInventoryChanged.AddDynamic(
            this, &UInventoryWidget::OnInventoryChanged);
    }

    RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
    if (!SlotContainer || !InventoryComp || !SlotWidgetClass) return;

    SlotContainer->ClearChildren();

    for (const FInventorySlot& InventorySlot : InventoryComp->Slots)
    {
        UInventorySlotWidget* SlotWidget =
            CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);

        if (!SlotWidget) continue;

        UTexture2D* Icon = nullptr;

        if (!InventorySlot.IsEmpty())
        {
            FItemTableRow* ItemData =
                InventoryComp->GetItemData(InventorySlot.ItemID);

            if (ItemData && !ItemData->Icon.IsNull())
            {
                Icon = ItemData->Icon.LoadSynchronous();
            }
        }

        SlotWidget->InitSlot(
            InventorySlot.ItemID,
            InventorySlot.Quantity,
            Icon,
            QuickSlotWidgetRef);

        SlotContainer->AddChild(SlotWidget);
    }

    if (QuickSlotWidgetRef)
    {
        QuickSlotWidgetRef->RefreshAllSlots();
    }
}

void UInventoryWidget::OnInventoryChanged()
{
    RefreshInventory();
}