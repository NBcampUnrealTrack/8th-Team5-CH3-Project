#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UConsumableInventoryComponent;
class UQuickSlotWidget;
class UWrapBox;
class UInventorySlotWidget;
class UQuickSlotSlotWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void InitInventory(UConsumableInventoryComponent* InInventory,
        UQuickSlotWidget* InQuickSlotWidget);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RefreshInventory();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot1;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot2;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot3;

    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot4;

    UPROPERTY(meta = (BindWidget))
    UWrapBox* SlotContainer;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
    TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

private:
    UPROPERTY()
    UConsumableInventoryComponent* InventoryComp;

    UPROPERTY()
    UQuickSlotWidget* QuickSlotWidgetRef;

    UFUNCTION()
    void OnInventoryChanged();
};