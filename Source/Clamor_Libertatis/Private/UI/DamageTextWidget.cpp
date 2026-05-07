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
		TB_Damage->SetText(FText::AsNumber((int32)Damage));
	}
}