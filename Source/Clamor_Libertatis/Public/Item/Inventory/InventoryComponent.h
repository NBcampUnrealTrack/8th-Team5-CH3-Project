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

    //아이템 배열 반환 - 비어있는 슬롯 제외, 전체 아이템 목록 만들 때 사용 예정
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    TArray<FInventorySlot> GetAllItems() const;

    // 슬롯 위치 포함 전체 저장
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    TArray<FInventorySlot> GetAllSlots() const;

    // 슬롯 위치 포함 전체 복원
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SetAllSlots(const TArray<FInventorySlot>& InSlots);

protected:
    virtual void BeginPlay() override;
    bool ValidateUseItem(FName ItemID, int32 Quantity, int32& OutSlotIdx);
    bool ConsumeQuantity(int32 SlotIdx, int32 Quantity);

private:
    int32 FindSlotByID(FName ItemID) const;
    int32 FindEmptySlot() const;
};