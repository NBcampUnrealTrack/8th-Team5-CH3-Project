#include "Item/Effect/HealEffectHandler.h"
#include "Combat/HealthComponent.h"

bool UHealEffectHandler::Execute_Implementation(AActor* Owner, const FItemTableRow& ItemData)
{
    UHealthComponent* HC = Owner->FindComponentByClass<UHealthComponent>();
    if (!HC) return false;
    HC->Heal(ItemData.EffectValue);
    return true;
}