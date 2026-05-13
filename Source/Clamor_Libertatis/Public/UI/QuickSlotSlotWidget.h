#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Item/ConsumableInventoryComponent.h"
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

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void SetKeyText(const FString& Key);

    void InitQuickSlotSlot(UQuickSlotWidget* InQuickSlotWidget);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UpdateSlot(FName ItemID, int32 Quantity, UTexture2D* Icon);

    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void ClearSlot();

    bool bIsInventorySlot = false; 

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuantityText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KeyText;

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
    UQuickSlotWidget* QuickSlotWidgetRef;

    FString CachedKeyString;

    FName CachedItemID = NAME_None;
    int32 CachedQuantity = 0;
};