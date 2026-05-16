#include "UI/Inventory/ItemPopupWidget.h"
#include "Item/ItemTableRow.h"
#include "Components/TextBlock.h"

void UItemPopupWidget::InitPopup(const FItemTableRow& ItemData)
{
    if (ItemNameText)
        ItemNameText->SetText(ItemData.ItemName);

    if (ItemDescriptionText)
        ItemDescriptionText->SetText(ItemData.Description);

    if (ItemEffectText)
    {
        FString EffectString;
        switch (ItemData.EffectType)
        {
        case EConsumableEffectType::Heal:
            EffectString = FString::Printf(TEXT("체력 회복: %.0f"), ItemData.EffectValue);
            break;
        case EConsumableEffectType::Bomb:
            EffectString = FString::Printf(TEXT("데미지: %.0f / 범위: %.0f / 스턴: %.1f초"),
                ItemData.EffectValue, ItemData.EffectRadius, ItemData.EffectDuration);
            break;
        case EConsumableEffectType::Mana:
            EffectString = FString::Printf(TEXT("마나 회복: %.0f"), ItemData.EffectValue);
            break;
        case EConsumableEffectType::Stamina:
            EffectString = FString::Printf(TEXT("스태미나 회복: %.0f"), ItemData.EffectValue);
            break;
        default:
            EffectString = TEXT("효과 없음");
            break;
        }
        ItemEffectText->SetText(FText::FromString(EffectString));
    }
}