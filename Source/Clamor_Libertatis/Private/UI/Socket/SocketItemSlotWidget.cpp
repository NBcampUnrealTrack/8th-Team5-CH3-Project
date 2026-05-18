#include "UI/Socket/SocketItemSlotWidget.h"
#include "UI/Inventory/MasterInventoryWidget.h"
#include "UI/Inventory/InventoryDragDropOperation.h"
#include "Item/Inventory/SocketItemInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void USocketItemSlotWidget::InitSlot(UMasterInventoryWidget* InParentWidget,
    USocketItemInventoryComponent* InInventory)
{
    ParentWidgetRef = InParentWidget;
    InventoryCompRef = InInventory;
}

void USocketItemSlotWidget::UpdateSlot(FName InItemID, UTexture2D* Icon)
{
    CachedItemID = InItemID;

    if (ItemIcon)
    {
        if (Icon)
        {
            ItemIcon->SetBrushFromTexture(Icon);
            ItemIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    }

    if (ItemNameText && InventoryCompRef)
    {
        FItemTableRow* ItemData = InventoryCompRef->GetItemData(InItemID);
        if (ItemData)
            ItemNameText->SetText(ItemData->ItemName);
    }
}

void USocketItemSlotWidget::ClearSlot()
{
    CachedItemID = NAME_None;
    if (ItemIcon) ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    if (ItemNameText) ItemNameText->SetText(FText::GetEmpty());
}

FReply USocketItemSlotWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (CachedItemID.IsNone()) return FReply::Unhandled();

    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        if (ParentWidgetRef)
            ParentWidgetRef->ShowItemInfo(CachedItemID);
        return FReply::Handled();
    }

    FReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(
        InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;

    if (Reply.IsEventHandled())
        Reply = Reply.CaptureMouse(GetCachedWidget().ToSharedRef());

    return Reply;
}

void USocketItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (CachedItemID.IsNone()) return;

    UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>(this);
    DragOp->ItemID = CachedItemID;
    DragOp->SourceInventorySlotIndex = SlotIndex;
    DragOp->SourceHotbarSlotIndex = -1;
    DragOp->Pivot = EDragPivot::CenterCenter;

    OutOperation = DragOp;
}

bool USocketItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry,
    const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    UInventoryDragDropOperation* DragOp =
        Cast<UInventoryDragDropOperation>(InOperation);
    if (!DragOp || !InventoryCompRef) return false;

    if (DragOp->SourceInventorySlotIndex == -1 &&
        DragOp->SourceHotbarSlotIndex == -1)
    {
        InventoryCompRef->AddItem(DragOp->ItemID, 1);
        InventoryCompRef->OnInventoryChanged.Broadcast();
        return true;
    }

    return false;
}