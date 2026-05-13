#include "Item/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] ItemDataTable이 연결되지 않았습니다."),
            *GetOwner()->GetName());
    }

    Slots.SetNum(MaxSlots);
}

bool UInventoryComponent::ConsumeQuantity(int32 SlotIdx, int32 Quantity)
{
    Slots[SlotIdx].Quantity -= Quantity;
    if (Slots[SlotIdx].Quantity <= 0)
    {
        Slots[SlotIdx] = FInventorySlot();
    }
    OnInventoryChanged.Broadcast();
    return true;
}

FItemTableRow* UInventoryComponent::GetItemData(FName ItemID) const
{
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemDataTable이 없습니다."));
        return nullptr;
    }

    FItemTableRow* Row = ItemDataTable->FindRow<FItemTableRow>(ItemID, TEXT("GetItemData"));
    if (!Row)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable에 없는 아이템 ID: %s"), *ItemID.ToString());
    }

    return Row;
}

EAddItemResult UInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
    if (ItemID.IsNone() || Quantity <= 0) return EAddItemResult::NotFound;

    FItemTableRow* ItemData = GetItemData(ItemID);
    if (!ItemData) return EAddItemResult::NotFound;

    if (ItemData->ItemType != AllowedItemType) return EAddItemResult::WrongType;

    int32 ExistingIdx = FindSlotByID(ItemID);
    if (ExistingIdx != INDEX_NONE)
    {
        Slots[ExistingIdx].Quantity += Quantity;
        OnInventoryChanged.Broadcast();
        return EAddItemResult::Success;
    }

    int32 EmptyIdx = FindEmptySlot();
    if (EmptyIdx == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("인벤토리가 가득 찼습니다."));
        return EAddItemResult::InventoryFull;
    }

    Slots[EmptyIdx].ItemID = ItemID;
    Slots[EmptyIdx].Quantity = Quantity;
    OnInventoryChanged.Broadcast();
    return EAddItemResult::Success;
}

bool UInventoryComponent::ValidateUseItem(FName ItemID, int32 Quantity, int32& OutSlotIdx)
{
    if (ItemID.IsNone() || Quantity <= 0) return false;

    OutSlotIdx = FindSlotByID(ItemID);
    if (OutSlotIdx == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("보유하지 않은 아이템: %s"), *ItemID.ToString());
        return false;
    }
    if (Slots[OutSlotIdx].Quantity < Quantity)
    {
        UE_LOG(LogTemp, Warning, TEXT("수량 부족"));
        return false;
    }

    FItemTableRow* ItemData = GetItemData(ItemID);
    if (!ItemData) return false;

    if (ItemData->ItemType == EItemType::Quest)
    {
        UE_LOG(LogTemp, Warning, TEXT("퀘스트 아이템은 사용할 수 없습니다: %s"), *ItemID.ToString());
        return false;
    }

    return true;
}

bool UInventoryComponent::UseItem(FName ItemID, int32 Quantity)
{
    int32 Idx;
    if (!ValidateUseItem(ItemID, Quantity, Idx)) return false;
    return ConsumeQuantity(Idx, Quantity);
}
bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    if (ItemID.IsNone() || Quantity <= 0) return false;

    int32 Idx = FindSlotByID(ItemID);
    if (Idx == INDEX_NONE)
    {
        UE_LOG(LogTemp, Warning, TEXT("보유하지 않은 아이템: %s"), *ItemID.ToString());
        return false;
    }

    FItemTableRow* ItemData = GetItemData(ItemID);
    if (ItemData && ItemData->ItemType == EItemType::Quest)
    {
        UE_LOG(LogTemp, Warning, TEXT("퀘스트 아이템은 버릴 수 없습니다: %s"), *ItemID.ToString());
        return false;
    }

    return ConsumeQuantity(Idx, Quantity);
}

bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
    return GetQuantityByID(ItemID) >= Quantity;
}

int32 UInventoryComponent::GetQuantityByID(FName ItemID) const
{
    int32 Idx = FindSlotByID(ItemID);
    return Idx != INDEX_NONE ? Slots[Idx].Quantity : 0;
}

void UInventoryComponent::ClearInventory()
{
    for (FInventorySlot& Slot : Slots)
    {
        Slot = FInventorySlot();
    }
    OnInventoryChanged.Broadcast();
}

int32 UInventoryComponent::FindSlotByID(FName ItemID) const
{
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (!Slots[i].IsEmpty() && Slots[i].ItemID == ItemID)
            return i;
    }
    return INDEX_NONE;
}

int32 UInventoryComponent::FindEmptySlot() const
{
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (Slots[i].IsEmpty()) return i;
    }
    return INDEX_NONE;
}