#include "UI/SkillCooldownWidget.h"
#include "Combat/SkillComponent.h"  
#include "Components/Image.h"         
#include "Components/TextBlock.h"    
#include "Materials/MaterialInstanceDynamic.h"

void USkillCooldownWidget::InitWithSkillComponent(USkillComponent* InSkillComponent)
{
    if (!InSkillComponent) return;

    SkillComponent = InSkillComponent;
    InSkillComponent->OnSkillCooldownStart.AddDynamic(
        this, &USkillCooldownWidget::OnCooldownStarted);

    if (CooldownMaterialBase && SkillIconImage)
    {
        CooldownMaterial = UMaterialInstanceDynamic::Create(CooldownMaterialBase, this);
        CooldownMaterial->SetTextureParameterValue(TEXT("SkillIcon"), SkillIconTexture);
        CooldownMaterial->SetScalarParameterValue(TEXT("CooldownRatio"), 0.f);
        SkillIconImage->SetBrushFromMaterial(CooldownMaterial);
    }

    //if (CooldownText)
        //CooldownText->SetVisibility(ESlateVisibility::Hidden);
}

void USkillCooldownWidget::OnCooldownStarted(float Duration)
{
    TotalCooldown = Duration;
    RemainingCooldown = Duration;
    bIsOnCooldown = true;

    //if (CooldownText)
        //CooldownText->SetVisibility(ESlateVisibility::Visible);
}

void USkillCooldownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (!bIsOnCooldown || !CooldownMaterial) return;

    RemainingCooldown = FMath::Max(0.f, RemainingCooldown - InDeltaTime);

    const float Ratio = (TotalCooldown > 0.f)? RemainingCooldown / TotalCooldown : 0.f;

    CooldownMaterial->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);

    //const int32 DisplaySec = FMath::CeilToInt(RemainingCooldown);
    //CooldownText->SetText(FText::AsNumber(DisplaySec));

    if (RemainingCooldown <= 0.f)
    {
        bIsOnCooldown = false;
       // CooldownText->SetVisibility(ESlateVisibility::Hidden);

        CooldownMaterial->SetScalarParameterValue(TEXT("CooldownRatio"), 0.f);
    }
}