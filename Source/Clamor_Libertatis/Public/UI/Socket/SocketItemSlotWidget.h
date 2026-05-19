#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Inventory/SocketItemInventoryComponent.h"
#include "SocketItemSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UMasterInventoryWidget;
class USocketItemInfoWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API USocketItemSlotWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void InitSlot(UMasterInventoryWidget* InParentWidget,
        USocketItemInventoryComponent* InInventory);
    void UpdateSlot(FName InItemID, UTexture2D* Icon);
    void ClearSlot();

    int32 SlotIndex = 0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DragDrop")
    TSubclassOf<UUserWidget> DragVisualClass;

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;
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
    FName CachedItemID = NAME_None;

    UPROPERTY()
    UMasterInventoryWidget* ParentWidgetRef;
    UPROPERTY()
    USocketItemInventoryComponent* InventoryCompRef;
    UPROPERTY()
    UTexture2D* CachedIcon = nullptr;
    
};