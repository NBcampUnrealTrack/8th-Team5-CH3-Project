#include "UI/PlayerHUDWidget.h"
#include "Combat/HealthComponent.h"
#include "UI/SkillCooldownWidget.h"
#include "Combat/SkillComponent.h"
#include "Components/Button.h"
#include "Character/BasePlayerController.h"

void UPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Log, TEXT("HUDWidget NativeConstruct 호출됨"));

    if (InventoryButton)
    {
        UE_LOG(LogTemp, Log, TEXT("InventoryButton 연결됨"));
        InventoryButton->OnClicked.AddDynamic(
            this, &UPlayerHUDWidget::OnInventoryButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("InventoryButton 없음 - 이름 확인 필요"));
    }
}

void UPlayerHUDWidget::OnInventoryButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("인벤토리 버튼 클릭됨"));

    ABasePlayerController* PC =
        Cast<ABasePlayerController>(GetOwningPlayer());
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("PC 캐스트 실패"));
        return;
    }

    PC->ShowInventory();
}

void UPlayerHUDWidget::InitWidget(UHealthComponent* InHealthComp)
{
    if (!InHealthComp) return;
    InHealthComp->OnHealthChanged.AddDynamic(
        this, &UPlayerHUDWidget::OnHealthChanged);
    InHealthComp->OnStaminaChanged.AddDynamic(
        this, &UPlayerHUDWidget::OnStaminaChanged);
    UpdateHP(
        InHealthComp->GetCurrentHealth(),
        InHealthComp->GetMaxHealth());
    UpdateStamina(
        InHealthComp->GetCurrentStamina(),
        InHealthComp->GetMaxStamina());
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