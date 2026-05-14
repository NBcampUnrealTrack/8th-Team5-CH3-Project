#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UHealthComponent;
class USkillComponent;
class USkillCooldownWidget;
class UButton;
class ABasePlayerController;

UCLASS()
class CLAMOR_LIBERTATIS_API UPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION()
    void InitWidget(UHealthComponent* InHealthComp);

    UFUNCTION()
    void InitSkillCooldown(USkillComponent* InSkillComp);

    UFUNCTION()
    void OnHealthChanged(float CurrentHealth, float MaxHealth);

    UFUNCTION()
    void OnStaminaChanged(float CurrentStamina, float MaxStamina);

    UFUNCTION()
    void OnManaChanged(float CurrentMana, float MaxMana);

    UFUNCTION(BlueprintImplementableEvent)
    void UpdateHP(float CurrentHealth, float MaxHealth);

    UFUNCTION(BlueprintImplementableEvent)
    void UpdateStamina(float CurrentStamina, float MaxStamina);

    UFUNCTION(BlueprintImplementableEvent)
    void UpdateMana(float CurrentMana, float MaxMana);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    USkillCooldownWidget* SkillCooldownWidget;

    UPROPERTY(meta = (BindWidget))
    UButton* StatButton;

private:
    UFUNCTION()
    void OnStatButtonClicked();
};