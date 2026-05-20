#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_CraftingSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingSlotClicked, FName, ItemID);

class UButton;
class UTextBlock;
class UImage;
class UW_ItemTooltip;

UCLASS()
class UW_CraftingSlot : public UUserWidget
{
    GENERATED_BODY()

public:
    void Init(FName InItemID, int32 InAmount, UDataTable* ItemTable);

    UPROPERTY(BlueprintAssignable, Category = "Crafting")
    FOnCraftingSlotClicked OnSlotClicked;

    UPROPERTY(BlueprintReadOnly, Category = "Crafting")
    FText ItemDescription;

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* SlotButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* AmountText;

    UPROPERTY(meta = (BindWidgetOptional))
    UImage* ItemIcon;

    UPROPERTY(EditDefaultsOnly, Category = "ToolTip")
    TSubclassOf<class UW_ItemTooltip> ItemDetailToolTip;

private:
    FName CachedItemID;

    UFUNCTION()
    void HandleSlotClicked();
};
