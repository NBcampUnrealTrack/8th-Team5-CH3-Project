#include "UI/InventoryWidget.h"
#include "UI/QuickSlotWidget.h"
#include "UI/QuickSlotSlotWidget.h"
#include "Item/ConsumableInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "Character/BasePlayerController.h"
#include "Components/Button.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnBackButtonClicked);

    AllSlots = {
        QuickSlot1, QuickSlot2, QuickSlot3, QuickSlot4,
        InvSlot5,  InvSlot6,  InvSlot7,  InvSlot8,
        InvSlot9,  InvSlot10, InvSlot11, InvSlot12,
        InvSlot13, InvSlot14, InvSlot15, InvSlot16,
        InvSlot17, InvSlot18, InvSlot19, InvSlot20
    };

    for (int32 i = 0; i < AllSlots.Num(); i++)
    {
        if (!AllSlots[i]) continue;
        AllSlots[i]->SlotIndex = i;
        AllSlots[i]->bIsInventorySlot = true;
    }

    if (QuickSlot1) { QuickSlot1->bIsInventorySlot = false; QuickSlot1->SetKeyText("1"); }
    if (QuickSlot2) { QuickSlot2->bIsInventorySlot = false; QuickSlot2->SetKeyText("2"); }
    if (QuickSlot3) { QuickSlot3->bIsInventorySlot = false; QuickSlot3->SetKeyText("3"); }
    if (QuickSlot4) { QuickSlot4->bIsInventorySlot = false; QuickSlot4->SetKeyText("4"); }
}

void UInventoryWidget::InitInventory(UConsumableInventoryComponent* InInventory,
    UQuickSlotWidget* InQuickSlotWidget)
{
    InventoryComp = InInventory;
    QuickSlotWidgetRef = InQuickSlotWidget;

    for (UQuickSlotSlotWidget* SlotWidget : AllSlots)
    {
        if (SlotWidget) SlotWidget->InitQuickSlotSlot(QuickSlotWidgetRef);
    }

    if (!InventoryComp->OnInventoryChanged.IsAlreadyBound(
        this, &UInventoryWidget::OnInventoryChanged))
    {
        InventoryComp->OnInventoryChanged.AddDynamic(
            this, &UInventoryWidget::OnInventoryChanged);
    }

    RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
    if (!InventoryComp) return;

    for (int32 i = 0; i < AllSlots.Num(); i++)
    {
        UQuickSlotSlotWidget* SlotWidget = AllSlots[i];
        if (!SlotWidget) continue;

        if (!InventoryComp->Slots.IsValidIndex(i) || InventoryComp->Slots[i].IsEmpty())
        {
            SlotWidget->ClearSlot();
            continue;
        }

        const FInventorySlot& InvSlot = InventoryComp->Slots[i];
        UTexture2D* Icon = nullptr;
        FItemTableRow* ItemData = InventoryComp->GetItemData(InvSlot.ItemID);
        if (ItemData && !ItemData->Icon.IsNull())
            Icon = ItemData->Icon.LoadSynchronous();

        SlotWidget->UpdateSlot(InvSlot.ItemID, InvSlot.Quantity, Icon);
    }

    if (QuickSlotWidgetRef)
        QuickSlotWidgetRef->RefreshAllSlots();
}

void UInventoryWidget::OnInventoryChanged()
{
    RefreshInventory();
}

void UInventoryWidget::OnBackButtonClicked()
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwningPlayer());
    if (!PC) return;
    PC->HideInventory();
}