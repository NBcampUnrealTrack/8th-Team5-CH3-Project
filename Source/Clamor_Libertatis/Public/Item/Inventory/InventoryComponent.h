#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Item/ItemTableRow.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    FName ItemID = NAME_None;
    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    int32 Quantity = 0;
    bool IsEmpty() const { return ItemID.IsNone() || Quantity <= 0; }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UENUM(BlueprintType)
enum class EAddItemResult : uint8
{
    Success,
    NotFound,
    WrongType,
    InventoryFull
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UInventoryComponent();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    UDataTable* ItemDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    EItemType AllowedItemType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    int32 MaxSlots = 10;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TArray<FInventorySlot> Slots;

    UPROPERTY(BlueprintAssignable, Category = "Inventory")
    FOnInventoryChanged OnInventoryChanged;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    EAddItemResult AddItem(FName ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    virtual bool UseItem(FName ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FName ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool HasItem(FName ItemID, int32 Quantity = 1) const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 GetQuantityByID(FName ItemID) const;

    FItemTableRow* GetItemData(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ClearInventory();

protected:
    virtual void BeginPlay() override;
    bool ValidateUseItem(FName ItemID, int32 Quantity, int32& OutSlotIdx);
    bool ConsumeQuantity(int32 SlotIdx, int32 Quantity);

private:
    int32 FindSlotByID(FName ItemID) const;
    int32 FindEmptySlot() const;
};