#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotWidget.generated.h"

class UQuickSlotSlotWidget;
class UConsumableInventoryComponent;

UCLASS()
class CLAMOR_LIBERTATIS_API UQuickSlotWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void InitQuickSlot(UConsumableInventoryComponent* InInventory);

    // 인벤토리 슬롯 0~3번이 퀵슬롯 1~4번에 자동 연동됨
    // AssignToQuickSlot은 인벤토리 창에서 드래그 드롭 용도로만 남김
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void AssignToQuickSlot(int32 SlotIndex, FName ItemID);

    // 키보드 1~4 입력 시 인벤토리 해당 슬롯 아이템 사용
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UseQuickSlot(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void RefreshAllSlots();

    void SwapQuickSlots(int32 FromIndex, int32 ToIndex);

    UConsumableInventoryComponent* GetInventoryComp() const { return InventoryComp; }

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* Slot1;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* Slot2;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* Slot3;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* Slot4;

private:
    UPROPERTY()
    UConsumableInventoryComponent* InventoryComp;

    void RefreshSlot(int32 InventorySlotIndex, UQuickSlotSlotWidget* SlotWidget);

    UFUNCTION()
    void OnInventoryChanged();


};