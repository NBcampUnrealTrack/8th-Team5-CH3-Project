#include "UI/DamageTextWidget.h"
#include "Components/TextBlock.h"

void UDamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDamageTextWidget::InitDamageText(float Damage)
{
	if (TB_Damage)
	{
		TB_Damage->SetText(
			FText::AsNumber((int32)Damage)
		);

		FSlateFontInfo FontInfo =
			TB_Damage->GetFont();

		float FontSize = FMath::Clamp(
			20.f + FMath::Sqrt(Damage) * 3.f,
			20.f,
			80.f
		);

		FontInfo.Size = FontSize;

		TB_Damage->SetFont(FontInfo);
	}
}