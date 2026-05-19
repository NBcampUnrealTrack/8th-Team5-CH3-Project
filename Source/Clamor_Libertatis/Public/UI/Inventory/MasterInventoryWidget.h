#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/QuickSlotSlotWidget.h"
#include "UI/Socket/SocketSlotWidget.h"
#include "UI/Socket/SocketItemSlotWidget.h"
#include "UI/Socket/SocketItemInfoWidget.h"
#include "UI/Inventory/ItemPopupWidget.h"
#include "MasterInventoryWidget.generated.h"

class UConsumableInventoryComponent;
class USocketItemInventoryComponent;
class UQuickSlotWidget;
class AWeaponBase;
class UButton;
class UTextBlock;

UCLASS()
class CLAMOR_LIBERTATIS_API UMasterInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void InitWidget(
        UConsumableInventoryComponent* InItemInventory,
        USocketItemInventoryComponent* InSocketInventory,
        UQuickSlotWidget* InQuickSlotWidget,
        AWeaponBase* InWeapon);

    UFUNCTION(BlueprintCallable)
    void RefreshAll();

    void ShowItemInfo(const FName& ItemID);
    void HideItemInfo();
    void ShowConsumableItemInfo(const FName& ItemID);
    void HideConsumableItemInfo();

    void RefreshStats();
    void UnequipSocketItemToInventory(FName ItemID);

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
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot5;
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot6;
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot7;
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot8;
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot9;
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot10;
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot11;
    UPROPERTY(meta = (BindWidgetOptional))
    UQuickSlotSlotWidget* InvSlot12;

    UPROPERTY(meta = (BindWidget))
    USocketSlotWidget* BladeSocketSlot;
    UPROPERTY(meta = (BindWidget))
    USocketSlotWidget* GripSocketSlot;

    UPROPERTY(meta = (BindWidget))
    USocketItemSlotWidget* SocketItemSlot1;
    UPROPERTY(meta = (BindWidget))
    USocketItemSlotWidget* SocketItemSlot2;
    UPROPERTY(meta = (BindWidget))
    USocketItemSlotWidget* SocketItemSlot3;
    UPROPERTY(meta = (BindWidget))
    USocketItemSlotWidget* SocketItemSlot4;
    UPROPERTY(meta = (BindWidget))
    USocketItemSlotWidget* SocketItemSlot5;

    UPROPERTY(meta = (BindWidget))
    USocketItemInfoWidget* ItemInfoPanel;

    UPROPERTY(meta = (BindWidget))
    UItemPopupWidget* ConsumableItemInfoPanel;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentHPText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentStaminaText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentManaText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* AttackDamageText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MoveSpeedText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* InventoryCountText;

    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

private:
    UPROPERTY()
    UConsumableInventoryComponent* ItemInventoryComp;
    UPROPERTY()
    USocketItemInventoryComponent* SocketInventoryComp;
    UPROPERTY()
    UQuickSlotWidget* QuickSlotWidgetRef;
    UPROPERTY()
    AWeaponBase* WeaponRef;

    TArray<UQuickSlotSlotWidget*> AllItemSlots;
    TArray<USocketItemSlotWidget*> AllSocketItemSlots;

    UFUNCTION()
    void OnBackButtonClicked();
    UFUNCTION()
    void OnItemInventoryChanged();
    UFUNCTION()
    void OnSocketInventoryChanged();

    void RefreshItemInventory();
    void RefreshSocketInventory();

};