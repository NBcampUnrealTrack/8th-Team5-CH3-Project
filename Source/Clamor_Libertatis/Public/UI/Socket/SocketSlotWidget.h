#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Combat/Weapon/WeaponSocketItemData.h"
#include "SocketSlotWidget.generated.h"

class UImage;
class UTextBlock;
class AWeaponBase;
class USocketItemInventoryComponent;
class UMasterInventoryWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API USocketSlotWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void InitSocketSlot(AWeaponBase* InWeapon, EWeaponSocketType InSocketType);
    void RefreshSlot();
    void SetInventoryComp(USocketItemInventoryComponent* InComp) { InventoryCompRef = InComp; }

    EWeaponSocketType SocketType = EWeaponSocketType::None;

    void SetMasterWidget(UMasterInventoryWidget* InMasterWidget) { MasterWidgetRef = InMasterWidget; }

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DragDrop")
    TSubclassOf<UUserWidget> DragVisualClass;
protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SocketNameText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent,
        UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;

private:
    UPROPERTY()
    AWeaponBase* WeaponRef;
    UPROPERTY()
    USocketItemInventoryComponent* InventoryCompRef;
    UPROPERTY()
    UMasterInventoryWidget* MasterWidgetRef = nullptr;
};