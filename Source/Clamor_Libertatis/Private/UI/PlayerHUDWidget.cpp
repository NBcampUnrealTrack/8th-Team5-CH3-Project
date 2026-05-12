#include "UI/PlayerHUDWidget.h"
#include "Combat/HealthComponent.h"

#include "UI/SkillCooldownWidget.h"
#include "Combat/SkillComponent.h"

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
void UPlayerHUDWidget::InitSkillCooldown(USkillComponent* InSkillComp)
{
    if (!InSkillComp || !SkillCooldownWidget) return;
    SkillCooldownWidget->InitWithSkillComponent(InSkillComp);
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