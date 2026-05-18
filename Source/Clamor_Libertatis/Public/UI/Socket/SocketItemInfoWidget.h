#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemTableRow.h"
#include "SocketItemInfoWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class CLAMOR_LIBERTATIS_API USocketItemInfoWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void ShowInfo(const FItemTableRow& ItemData);
    void HideInfo();

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemNameText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemDescriptionText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ElementText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ModifierText;
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIconImage;
};