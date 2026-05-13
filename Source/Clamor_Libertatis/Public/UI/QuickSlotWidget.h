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

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void AssignToQuickSlot(int32 SlotIndex, FName ItemID);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    FName GetQuickSlotItem(int32 SlotIndex) const;

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UseQuickSlot(int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void RefreshAllSlots();

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

    TArray<FName> QuickSlots;

    void RefreshSlot(int32 SlotIndex, UQuickSlotSlotWidget* SlotWidget);

    UFUNCTION()
    void OnInventoryChanged();
};