#include "UI/PlayerHUDWidget.h"
#include "Combat/HealthComponent.h"
#include "UI/SkillCooldownWidget.h"
#include "Combat/SkillComponent.h"
#include "Components/Button.h"
#include "Character/BasePlayerController.h"

void UPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StatButton)
    {
        StatButton->OnClicked.AddDynamic(
            this, &UPlayerHUDWidget::OnStatButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("StatButton 없음 - 이름 확인 필요"));
    }
}

void UPlayerHUDWidget::OnStatButtonClicked()
{
    ABasePlayerController* PC =
        Cast<ABasePlayerController>(GetOwningPlayer());
    if (!PC) return;
    PC->ShowStatWidget();
}

void UPlayerHUDWidget::InitWidget(UHealthComponent* InHealthComp)
{
    if (!InHealthComp) return;

    InHealthComp->OnHealthChanged.AddDynamic(
        this, &UPlayerHUDWidget::OnHealthChanged);
    InHealthComp->OnStaminaChanged.AddDynamic(
        this, &UPlayerHUDWidget::OnStaminaChanged);
    InHealthComp->OnManaChanged.AddDynamic(
        this, &UPlayerHUDWidget::OnManaChanged);

    UpdateHP(InHealthComp->GetCurrentHealth(), InHealthComp->GetMaxHealth());
    UpdateStamina(InHealthComp->GetCurrentStamina(), InHealthComp->GetMaxStamina());
    UpdateMana(InHealthComp->GetCurrentMana(), InHealthComp->GetMaxMana());
}

void UPlayerHUDWidget::InitSkillCooldown(USkillComponent* InSkillComp)
{
    if (!InSkillComp || !SkillCooldownWidget) return;
    SkillCooldownWidget->InitWithSkillComponent(InSkillComp);
}

void UPlayerHUDWidget::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
    UpdateHP(CurrentHealth, MaxHealth);
}

void UPlayerHUDWidget::OnStaminaChanged(float CurrentStamina, float MaxStamina)
{
    UpdateStamina(CurrentStamina, MaxStamina);
}

void UPlayerHUDWidget::OnManaChanged(float CurrentMana, float MaxMana)
{
    UpdateMana(CurrentMana, MaxMana);
}