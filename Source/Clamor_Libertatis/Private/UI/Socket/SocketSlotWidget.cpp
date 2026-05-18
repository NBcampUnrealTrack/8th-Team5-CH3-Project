#include "UI/Socket/SocketSlotWidget.h"
#include "Combat/Weapon/WeaponBase.h"
#include "UI/Inventory/MasterInventoryWidget.h"
#include "Item/Inventory/SocketItemInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "UI/Inventory/InventoryDragDropOperation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void USocketSlotWidget::InitSocketSlot(AWeaponBase* InWeapon, EWeaponSocketType InSocketType)
{
    WeaponRef = InWeapon;
    SocketType = InSocketType;
    RefreshSlot();
}

void USocketSlotWidget::RefreshSlot()
{
    if (!WeaponRef) return;

    if (SocketNameText)
    {
        FString SocketName = SocketType == EWeaponSocketType::Blade
            ? TEXT("Blade") : TEXT("Grip");
        SocketNameText->SetText(FText::FromString(SocketName));
    }

    UWeaponSocketItemData* EquippedItem = WeaponRef->GetEquippedSocketItem(SocketType);
    if (EquippedItem)
    {
        if (ItemNameText)
            ItemNameText->SetText(EquippedItem->DisplayName);
        if (ItemIcon)
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        if (ItemNameText)
            ItemNameText->SetText(FText::FromString(TEXT("Empty")));
        if (ItemIcon)
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
    }
}

FReply USocketSlotWidget::NativeOnMouseButtonDown(
    const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (!WeaponRef) return FReply::Unhandled();

    UWeaponSocketItemData* EquippedItem = WeaponRef->GetEquippedSocketItem(SocketType);
    if (!EquippedItem) return FReply::Unhandled();

    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        if (MasterWidgetRef)
            MasterWidgetRef->ShowItemInfo(EquippedItem->InventoryItemID);
        return FReply::Handled();
    }

    FReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(
        InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;

    if (Reply.IsEventHandled())
        Reply = Reply.CaptureMouse(GetCachedWidget().ToSharedRef());

    return Reply;
}

void USocketSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry,
    const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (!WeaponRef || !InventoryCompRef) return;

    UWeaponSocketItemData* EquippedItem = WeaponRef->GetEquippedSocketItem(SocketType);
    if (!EquippedItem) return;

    WeaponRef->UnequipSocketItem(SocketType);
    RefreshSlot();

    if (MasterWidgetRef)
        MasterWidgetRef->RefreshStats();

    UInventoryDragDropOperation* DragOp = NewObject<UInventoryDragDropOperation>(this);
    DragOp->ItemID = EquippedItem->InventoryItemID;
    DragOp->SourceInventorySlotIndex = -1;
    DragOp->SourceHotbarSlotIndex = -1;
    DragOp->Pivot = EDragPivot::CenterCenter;

    OutOperation = DragOp;
}

bool USocketSlotWidget::NativeOnDrop(const FGeometry& InGeometry,
    const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    UInventoryDragDropOperation* DragOp =
        Cast<UInventoryDragDropOperation>(InOperation);
    if (!DragOp || !WeaponRef || !InventoryCompRef) return false;

    FItemTableRow* ItemData = InventoryCompRef->GetItemData(DragOp->ItemID);
    if (!ItemData) return false;
    if (ItemData->ItemType != EItemType::SocketItem) return false;

    UWeaponSocketItemData* SocketData = ItemData->SocketItemData.LoadSynchronous();
    if (!SocketData) return false;

    if (!WeaponRef->CanEquipSocketItem(SocketData, SocketType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Not compatible socket item"));
        return false;
    }

    UWeaponSocketItemData* PrevItem = WeaponRef->UnequipSocketItem(SocketType);
    if (PrevItem)
        InventoryCompRef->AddItem(PrevItem->InventoryItemID, 1);

    WeaponRef->EquipSocketItem(SocketData, SocketType);
    InventoryCompRef->RemoveItem(DragOp->ItemID, 1);
    InventoryCompRef->OnInventoryChanged.Broadcast();

    RefreshSlot();

    if (MasterWidgetRef)
        MasterWidgetRef->RefreshStats();

    return true;
}