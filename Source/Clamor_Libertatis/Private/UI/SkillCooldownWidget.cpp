#include "UI/SkillCooldownWidget.h"
#include "Combat/SkillComponent.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void USkillCooldownWidget::InitWithSkillComponent(USkillComponent* InSkillComponent, FName InSkillName)
{
    if (!InSkillComponent) return;
    SkillComponent = InSkillComponent;
    BoundSkillName = InSkillName;

    InSkillComponent->OnSkillCooldownStart.AddDynamic(
        this, &USkillCooldownWidget::OnCooldownStarted);

    if (CooldownMaterialBase && SkillIconImage)
    {
        CooldownMaterial = UMaterialInstanceDynamic::Create(CooldownMaterialBase, this);
        CooldownMaterial->SetTextureParameterValue(TEXT("SkillIcon"), SkillIconTexture);
        CooldownMaterial->SetScalarParameterValue(TEXT("CooldownRatio"), 0.f);
        SkillIconImage->SetBrushFromMaterial(CooldownMaterial);
    }
}

void USkillCooldownWidget::OnCooldownStarted(FName SkillName, float Duration)
{
    if (SkillName != BoundSkillName) return;
    TotalCooldown = Duration;
    CooldownStartTime = GetWorld()->GetTimeSeconds();
    bIsOnCooldown = true;
}

void USkillCooldownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (!bIsOnCooldown || !CooldownMaterial) return;

    const float Elapsed = GetWorld()->GetTimeSeconds() - CooldownStartTime;
    RemainingCooldown = FMath::Max(0.f, TotalCooldown - Elapsed);

    const float Ratio = (TotalCooldown > 0.f) ? RemainingCooldown / TotalCooldown : 0.f;
    CooldownMaterial->SetScalarParameterValue(TEXT("CooldownRatio"), Ratio);

    if (RemainingCooldown <= 0.f)
    {
        bIsOnCooldown = false;
        CooldownMaterial->SetScalarParameterValue(TEXT("CooldownRatio"), 0.f);
    }
}