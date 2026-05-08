#include "UI/PlayerHUDWidget.h"
#include "Combat/HealthComponent.h"

void UPlayerHUDWidget::InitWidget(UHealthComponent* InHealthComp)
{
    if (!InHealthComp) return;

    InHealthComp->OnHealthChanged.AddDynamic(
        this,
        &UPlayerHUDWidget::OnHealthChanged
    );

    InHealthComp->OnStaminaChanged.AddDynamic(
        this,
        &UPlayerHUDWidget::OnStaminaChanged
    );

    UpdateHP(
        InHealthComp->GetCurrentHealth(),
        InHealthComp->GetMaxHealth()
    );

    UpdateStamina(
        InHealthComp->GetCurrentStamina(),
        InHealthComp->GetMaxStamina()
    );
}

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