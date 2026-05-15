#include "Item/Inventory/SocketItemInventoryComponent.h"

USocketItemInventoryComponent::USocketItemInventoryComponent()
{
    AllowedItemType = EItemType::SocketItem;
    MaxSlots = 20;

    static ConstructorHelpers::FObjectFinder<UDataTable> DTFinder(
        TEXT("/Game/Item/DT_ItemTable.DT_ItemTable"));
    if (DTFinder.Succeeded())
        ItemDataTable = DTFinder.Object;
}

bool USocketItemInventoryComponent::UseItem(FName ItemID, int32 Quantity)
{
    return false;
}