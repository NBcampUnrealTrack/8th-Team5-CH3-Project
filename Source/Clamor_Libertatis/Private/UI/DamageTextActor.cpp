#include "UI/DamageTextActor.h"
#include "Components/WidgetComponent.h"
#include "UI/DamageTextWidget.h"

ADamageTextActor::ADamageTextActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	RootComponent = WidgetComp;

	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
}

void ADamageTextActor::BeginPlay()
{
	Super::BeginPlay();

	DamageWidget = Cast<UDamageTextWidget>(
		WidgetComp->GetUserWidgetObject()
	);
}

void ADamageTextActor::InitDamage(float Damage)
{
	if (DamageWidget)
	{
		DamageWidget->InitDamageText(Damage);
	}

	SetLifeSpan(2.f);
}