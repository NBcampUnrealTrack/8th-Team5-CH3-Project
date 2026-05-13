#include "UI/QuickSlotSlotWidget.h"
#include "UI/QuickSlotWidget.h"
#include "UI/InventoryDragDropOperation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UQuickSlotSlotWidget::InitQuickSlotSlot(UQuickSlotWidget* InQuickSlotWidget)
{
    QuickSlotWidgetRef = InQuickSlotWidget;
}

void UQuickSlotSlotWidget::UpdateSlot(FName ItemID, int32 Quantity, UTexture2D* Icon)
{
    if (ItemIcon && Icon)
    {
        ItemIcon->SetBrushFromTexture(Icon);
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
    }
    else if (ItemIcon)
    {
        ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    }

    if (QuantityText)
    {
        QuantityText->SetText(FText::AsNumber(Quantity));
        QuantityText->SetVisibility(Quantity > 1
            ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);
    }
}

void UQuickSlotSlotWidget::ClearSlot()
{
    if (ItemIcon) ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    if (QuantityText) QuantityText->SetVisibility(ESlateVisibility::Collapsed);
}

bool UQuickSlotSlotWidget::NativeOnDrop(const FGeometry& InGeometry,
    const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    UInventoryDragDropOperation* DragOp =
        Cast<UInventoryDragDropOperation>(InOperation);
    if (!DragOp || !QuickSlotWidgetRef) return false;

    QuickSlotWidgetRef->AssignToQuickSlot(SlotIndex, DragOp->ItemID);
    return true;
}