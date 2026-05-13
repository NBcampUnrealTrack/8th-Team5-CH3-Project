#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UQuickSlotWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API UInventorySlotWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void InitSlot(FName InItemID, int32 Quantity, UTexture2D* Icon,
        UQuickSlotWidget* InQuickSlotWidget);

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuantityText;

    virtual void NativeConstruct() override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent,
        UDragDropOperation*& OutOperation) override;

private:
    FName ItemID;

    UPROPERTY()
    UQuickSlotWidget* QuickSlotWidgetRef;
};