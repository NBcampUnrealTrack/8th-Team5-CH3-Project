#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillCooldownWidget.generated.h"

class USkillComponent;
class UImage;
//class UTextBlock;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class CLAMOR_LIBERTATIS_API USkillCooldownWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void InitWithSkillComponent(USkillComponent* InSkillComponent);

protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(meta = (BindWidget))
    UImage* SkillIconImage;        

    //UPROPERTY(meta = (BindWidget))
    //UTextBlock* CooldownText;        

    UPROPERTY()
    UMaterialInstanceDynamic* CooldownMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    UMaterialInterface* CooldownMaterialBase;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    UTexture2D* SkillIconTexture;

private:
    UFUNCTION()
    void OnCooldownStarted(float Duration);

    TWeakObjectPtr<USkillComponent> SkillComponent;
    float TotalCooldown = 0.f;
    float RemainingCooldown = 0.f;
    bool  bIsOnCooldown = false;
};