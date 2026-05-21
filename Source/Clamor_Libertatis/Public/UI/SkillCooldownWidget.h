#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCooldownWidget.generated.h"

class USkillComponent;
class UImage;
class UTexture2D;
class UMaterialInstanceDynamic;

UCLASS()
class USkillCooldownWidget : public UUserWidget
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void InitWithSkillComponent(USkillComponent* InSkillComponent, FName InSkillName);

protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SkillIconImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TObjectPtr<UTexture2D> SkillIconTexture;

    UPROPERTY(EditDefaultsOnly, Category = "Skill")
    TObjectPtr<UMaterialInterface> CooldownMaterialBase;

private:
    UFUNCTION()
    void OnCooldownStarted(FName SkillName, float Duration);

    UPROPERTY()
    TObjectPtr<USkillComponent> SkillComponent;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> CooldownMaterial;

    FName BoundSkillName; 

    float TotalCooldown = 0.f;
    float CooldownStartTime = 0.f;
    float RemainingCooldown = 0.f;
    bool bIsOnCooldown = false;
};