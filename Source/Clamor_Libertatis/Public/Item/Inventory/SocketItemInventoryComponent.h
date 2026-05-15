#pragma once
#include "CoreMinimal.h"
#include "Item/Inventory/InventoryComponent.h"
#include "SocketItemInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API USocketItemInventoryComponent : public UInventoryComponent
{
    GENERATED_BODY()
public:
    USocketItemInventoryComponent();
    virtual bool UseItem(FName ItemID, int32 Quantity = 1) override;
};