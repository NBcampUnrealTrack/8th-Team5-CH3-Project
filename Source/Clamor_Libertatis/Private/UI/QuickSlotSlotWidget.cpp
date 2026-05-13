#include "UI/QuickSlotSlotWidget.h"
#include "UI/QuickSlotWidget.h"
#include "UI/InventoryDragDropOperation.h"
#include "Item/ConsumableInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UQuickSlotSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    ClearSlot();
}

void UQuickSlotSlotWidget::SetKeyText(const FString& Key)
{
    CachedKeyString = Key;
}

void UQuickSlotSlotWidget::InitQuickSlotSlot(UQuickSlotWidget* InQuickSlotWidget)
{
    QuickSlotWidgetRef = InQuickSlotWidget;
}

void UQuickSlotSlotWidget::UpdateSlot(FName ItemID, int32 Quantity, UTexture2D* Icon)
{
    CachedItemID = ItemID;
    CachedQuantity = Quantity;

    if (ItemIcon)
    {
        if (Icon)
        {
            ItemIcon->SetBrushFromTexture(Icon);
            ItemIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemIcon->SetBrushFromTexture(nullptr);
            ItemIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (QuantityText)
    {
        QuantityText->SetText(FText::AsNumber(Quantity));
        QuantityText->SetVisibility(Quantity >= 1
            ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);
    }

    if (KeyText)
    {
        if (!CachedKeyString.IsEmpty())
            KeyText->SetText(FText::FromString(CachedKeyString));
        KeyText->SetVisibility(ESlateVisibility::Visible);
    }
}

void UQuickSlotSlotWidget::ClearSlot()
{
    CachedItemID = NAME_None;
    CachedQuantity = 0;

    if (ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(nullptr);
        ItemIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
        ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    }
    if (QuantityText)
    {
        QuantityText->SetText(FText::GetEmpty());
        QuantityText->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (KeyText)
    {
        KeyText->SetVisibility(ESlateVisibility::Collapsed);
    }
}

FReply UQuickSlotSlotWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

    if (CachedItemID.IsNone()) return FReply::Unhandled();

    FReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(
        InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;

    if (Reply.IsEventHandled())
        Reply = Reply.CaptureMouse(GetCachedWidget().ToSharedRef());

    return Reply;
}

void UQuickSlotSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (CachedItemID.IsNone()) return;

    UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>(this);
    DragOp->ItemID = CachedItemID;
    DragOp->Pivot = EDragPivot::MouseDown;

    if (bIsInventorySlot)
    {
        DragOp->SourceInventorySlotIndex = SlotIndex;
        DragOp->SourceHotbarSlotIndex = -1;
    }
    else
    {
        DragOp->SourceHotbarSlotIndex = SlotIndex;
        DragOp->SourceInventorySlotIndex = -1;
    }

    UQuickSlotSlotWidget* DragVisual =
        CreateWidget<UQuickSlotSlotWidget>(GetOwningPlayer(), GetClass());
    if (DragVisual)
    {
        UTexture2D* Icon = nullptr;
        if (ItemIcon)
            Icon = Cast<UTexture2D>(ItemIcon->GetBrush().GetResourceObject());

        DragVisual->UpdateSlot(CachedItemID, CachedQuantity, Icon);
    }
    DragOp->DefaultDragVisual = DragVisual;
    OutOperation = DragOp;
}

bool UQuickSlotSlotWidget::NativeOnDrop(const FGeometry& InGeometry,
    const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    UInventoryDragDropOperation* DragOp =
        Cast<UInventoryDragDropOperation>(InOperation);
    if (!DragOp) return false;

    // 인벤토리 슬롯끼리 스왑 (QuickSlotWidget 불필요)
    if (bIsInventorySlot && DragOp->SourceInventorySlotIndex >= 0)
    {
        if (!QuickSlotWidgetRef) return false;
        UConsumableInventoryComponent* InvComp = QuickSlotWidgetRef->GetInventoryComp();
        if (!InvComp) return false;

        InvComp->Slots.Swap(DragOp->SourceInventorySlotIndex, SlotIndex);
        InvComp->OnInventoryChanged.Broadcast();
        return true;
    }

    if (bIsInventorySlot && DragOp->SourceHotbarSlotIndex >= 0)
    {
        if (!QuickSlotWidgetRef) return false;
        QuickSlotWidgetRef->SwapQuickSlots(DragOp->SourceHotbarSlotIndex, SlotIndex);
        return true;
    }

    if (!bIsInventorySlot && DragOp->SourceHotbarSlotIndex >= 0)
    {
        if (!QuickSlotWidgetRef) return false;
        QuickSlotWidgetRef->SwapQuickSlots(DragOp->SourceHotbarSlotIndex, SlotIndex);
        return true;
    }
    if (!bIsInventorySlot && DragOp->SourceInventorySlotIndex >= 0)
    {
        if (!QuickSlotWidgetRef) return false;
        QuickSlotWidgetRef->AssignToQuickSlot(SlotIndex, DragOp->ItemID);
        return true;
    }

    return false;
}