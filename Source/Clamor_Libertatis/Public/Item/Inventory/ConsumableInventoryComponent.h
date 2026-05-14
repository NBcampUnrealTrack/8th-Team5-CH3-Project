#pragma once
#include "CoreMinimal.h"
#include "Item/Inventory/InventoryComponent.h"
#include "Item/Effect/ItemEffectHandler.h"
#include "ConsumableInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UConsumableInventoryComponent : public UInventoryComponent
{
    GENERATED_BODY()
public:
    UConsumableInventoryComponent();
    virtual bool UseItem(FName ItemID, int32 Quantity = 1) override;

private:
    UPROPERTY()
    TMap<EConsumableEffectType, TSubclassOf<UItemEffectHandler>> EffectHandlerClasses;
};