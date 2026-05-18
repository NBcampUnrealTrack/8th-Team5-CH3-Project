#include "UI/Socket/SocketItemInfoWidget.h"
#include "Item/ItemTableRow.h"
#include "Combat/Weapon/WeaponSocketItemData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USocketItemInfoWidget::ShowInfo(const FItemTableRow& ItemData)
{
    SetVisibility(ESlateVisibility::Visible);

    if (ItemNameText)
        ItemNameText->SetText(ItemData.ItemName);
    if (ItemDescriptionText)
        ItemDescriptionText->SetText(ItemData.Description);

    if (ItemIconImage && !ItemData.Icon.IsNull())
    {
        UTexture2D* Icon = ItemData.Icon.LoadSynchronous();
        if (Icon)
            ItemIconImage->SetBrushFromTexture(Icon);
    }

    if (!ItemData.SocketItemData.IsNull())
    {
        UWeaponSocketItemData* SocketData = ItemData.SocketItemData.LoadSynchronous();
        if (SocketData)
        {
            if (ElementText)
            {
                FString ElementStr;
                switch (SocketData->Element)
                {
                case EWeaponSocketElement::Fire:      ElementStr = TEXT("Fire");      break;
                case EWeaponSocketElement::Ice:       ElementStr = TEXT("Ice");       break;
                case EWeaponSocketElement::Lightning: ElementStr = TEXT("Lightning"); break;
                case EWeaponSocketElement::Poison:    ElementStr = TEXT("Poison");    break;
                case EWeaponSocketElement::Holy:      ElementStr = TEXT("Holy");      break;
                case EWeaponSocketElement::Dark:      ElementStr = TEXT("Dark");      break;
                default:                              ElementStr = TEXT("None");      break;
                }
                ElementText->SetText(FText::FromString(ElementStr));
            }

            if (ModifierText)
            {
                FString ModStr;
                const FWeaponSocketItemModifier& Mod = SocketData->Modifier;
                if (Mod.AdditiveDamage != 0.f)
                    ModStr += FString::Printf(TEXT("Add Damage: +%.0f\n"), Mod.AdditiveDamage);
                if (Mod.DamageMultiplier != 1.f)
                    ModStr += FString::Printf(TEXT("Damage: x%.2f\n"), Mod.DamageMultiplier);
                if (Mod.AttackSpeedMultiplier != 1.f)
                    ModStr += FString::Printf(TEXT("Attack Speed: x%.2f\n"), Mod.AttackSpeedMultiplier);
                if (Mod.StaminaCostMultiplier != 1.f)
                    ModStr += FString::Printf(TEXT("Stamina Cost: x%.2f\n"), Mod.StaminaCostMultiplier);
                if (Mod.ManaCostMultiplier != 1.f)
                    ModStr += FString::Printf(TEXT("Mana Cost: x%.2f\n"), Mod.ManaCostMultiplier);

                ModifierText->SetText(FText::FromString(ModStr));
            }
        }
    }
}

void USocketItemInfoWidget::HideInfo()
{
    SetVisibility(ESlateVisibility::Collapsed);
}