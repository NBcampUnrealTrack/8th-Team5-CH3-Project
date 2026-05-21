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
        UE_LOG(LogTemp, Log, TEXT("StatButton 연결됨"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("StatButton 없음 - 이름 확인 필요"));
    }
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
    if (!InSkillComp) return;

    if (SkillWidget0) SkillWidget0->InitWithSkillComponent(InSkillComp, "Fireball");
    if (SkillWidget1) SkillWidget1->InitWithSkillComponent(InSkillComp, "LeapStrike");
    if (SkillWidget2) SkillWidget2->InitWithSkillComponent(InSkillComp, "SpinSlash");
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

void UPlayerHUDWidget::OnStatButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("StatButton 클릭됨")); 

    ABasePlayerController* PC =
        Cast<ABasePlayerController>(GetOwningPlayer());
    UE_LOG(LogTemp, Warning, TEXT("PC 캐스트: %s"), PC ? TEXT("성공") : TEXT("실패")); 

    if (!PC) return;
    PC->ShowMasterInventory();
}