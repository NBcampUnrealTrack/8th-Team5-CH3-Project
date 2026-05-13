#include "UI/InventorySlotWidget.h"
#include "UI/QuickSlotWidget.h"
#include "UI/InventoryDragDropOperation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UInventorySlotWidget::InitSlot(FName InItemID, int32 Quantity,
    UTexture2D* Icon,
    UQuickSlotWidget* InQuickSlotWidget)
{
    ItemID = InItemID;
    QuickSlotWidgetRef = InQuickSlotWidget;

    if (ItemIcon)
    {
        if (Icon)
        {
            ItemIcon->SetBrushFromTexture(Icon);
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (QuantityText)
    {
        if (!InItemID.IsNone())
        {
            QuantityText->SetText(FText::AsNumber(Quantity));
            QuantityText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            QuantityText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent)
{
    if (ItemID.IsNone()) return FReply::Unhandled();

    return UWidgetBlueprintLibrary::DetectDragIfPressed(
        InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent,
    UDragDropOperation*& OutOperation)
{
    UInventoryDragDropOperation* DragOp =
        NewObject<UInventoryDragDropOperation>(this);
    DragOp->ItemID = ItemID;

    if (ItemIcon)
    {
        DragOp->DefaultDragVisual = ItemIcon;
    }

    OutOperation = DragOp;
}