#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UQuickSlotWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API UQuickSlotSlotWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "QuickSlot")
    int32 SlotIndex = 0;

    void InitQuickSlotSlot(UQuickSlotWidget* InQuickSlotWidget);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UpdateSlot(FName ItemID, int32 Quantity, UTexture2D* Icon);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void ClearSlot();

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuantityText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KeyText;

    virtual bool NativeOnDrop(const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;

private:
    UPROPERTY()
    UQuickSlotWidget* QuickSlotWidgetRef;
};