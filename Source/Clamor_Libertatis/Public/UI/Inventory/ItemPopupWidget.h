#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemTableRow.h"
#include "ItemPopupWidget.generated.h"

class UTextBlock;

UCLASS()
class CLAMOR_LIBERTATIS_API UItemPopupWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void InitPopup(const FItemTableRow& ItemData);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescriptionText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemEffectText;
};