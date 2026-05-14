#include "Item/Inventory/ConsumableInventoryComponent.h"
#include "Item/Effect/HealEffectHandler.h"
#include "Item/Effect/BombEffectHandler.h"

UConsumableInventoryComponent::UConsumableInventoryComponent()
{
    AllowedItemType = EItemType::Consumable;
    MaxSlots = 20;

    static ConstructorHelpers::FObjectFinder<UDataTable> DTFinder(
        TEXT("/Game/Item/DT_ItemTable.DT_ItemTable"));
    if (DTFinder.Succeeded())
    {
        ItemDataTable = DTFinder.Object;
    }

    EffectHandlerClasses.Add(EConsumableEffectType::Heal, UHealEffectHandler::StaticClass());
    EffectHandlerClasses.Add(EConsumableEffectType::Bomb, UBombEffectHandler::StaticClass());
    // 나중에 마나/스태미너 추가할 때 여기만 추가
    // EffectHandlerClasses.Add(EConsumableEffectType::Mana, UManaEffectHandler::StaticClass());
}

bool UConsumableInventoryComponent::UseItem(FName ItemID, int32 Quantity)
{
    int32 Idx;
    if (!ValidateUseItem(ItemID, Quantity, Idx)) return false;

    FItemTableRow* ItemData = GetItemData(ItemID);
    if (!ItemData) return false;

    TSubclassOf<UItemEffectHandler>* HandlerClass =
        EffectHandlerClasses.Find(ItemData->EffectType);
    if (HandlerClass && *HandlerClass)
    {
        UItemEffectHandler* Handler = NewObject<UItemEffectHandler>(this, *HandlerClass);
        Handler->Execute(GetOwner(), *ItemData);
    }

    return ConsumeQuantity(Idx, Quantity);
}