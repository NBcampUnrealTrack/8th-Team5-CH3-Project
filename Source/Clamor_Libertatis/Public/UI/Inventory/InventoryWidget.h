#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UConsumableInventoryComponent;
class UQuickSlotWidget;
class UButton;
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

    // Äü½½·Ô 1-4 (HUD¿¡µµ Ç¥½Ã)
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot1;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot2;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot3;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* QuickSlot4;

    // ÀÎº¥Åä¸® Àü¿ë ½½·Ô 5-20
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot5;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot6;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot7;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot8;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot9;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot10;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot11;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot12;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot13;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot14;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot15;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot16;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot17;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot18;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot19;
    UPROPERTY(meta = (BindWidget))
    UQuickSlotSlotWidget* InvSlot20;

    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    UButton* StatButton;
private:
    UPROPERTY()
    UConsumableInventoryComponent* InventoryComp;

    UPROPERTY()
    UQuickSlotWidget* QuickSlotWidgetRef;

    TArray<UQuickSlotSlotWidget*> AllSlots;

    UFUNCTION()
    void OnInventoryChanged();
    UFUNCTION()
    void OnBackButtonClicked();
    UFUNCTION()
    void OnStatButtonClicked();
};