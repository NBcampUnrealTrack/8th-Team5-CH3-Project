#include "UI/PlayerHUDWidget.h"

void UPlayerHUDWidget::OnHealthChanged(
    float CurrentHealth,
    float MaxHealth
)
{
    UpdateHP(CurrentHealth, MaxHealth);
}

void UPlayerHUDWidget::OnStaminaChanged(
    float CurrentStamina,
    float MaxStamina
)
{
    UpdateStamina(CurrentStamina, MaxStamina);
}