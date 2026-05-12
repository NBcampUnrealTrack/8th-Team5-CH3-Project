#include "UI/UIManager.h"
#include "UI/SkillCooldownWidget.h" 
#include "Combat/SkillComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void UUIManager::Initialize(APlayerController* InOwner)
{
	OwnerController = InOwner;
}

void UUIManager::ShowWidget(EUIType Type)
{
	if (UUserWidget* W = GetOrCreate(Type))
	{
		W->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUIManager::HideWidget(EUIType Type)
{
	if (FWidgetEntry* Entry = WidgetEntries.Find(Type))
	{
		if (Entry->WidgetInstance)
		{
			Entry->WidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

UUserWidget* UUIManager::GetWidget(EUIType Type) const
{
	if (const FWidgetEntry* Entry = WidgetEntries.Find(Type))
	{
		return Entry->WidgetInstance.Get();
	}
	return nullptr;
}

bool UUIManager::IsWidgetVisible(EUIType Type) const
{
	if (const FWidgetEntry* Entry = WidgetEntries.Find(Type))
	{
		return Entry->WidgetInstance && Entry->WidgetInstance->IsVisible();
	}
	return false;
}

UUserWidget* UUIManager::GetOrCreate(EUIType Type)
{
	FWidgetEntry* Entry = WidgetEntries.Find(Type);
	if (!Entry || !Entry->WidgetClass) return nullptr;

	if (!Entry->WidgetInstance)
	{
		Entry->WidgetInstance = CreateWidget<UUserWidget>(OwnerController.Get(), Entry->WidgetClass);
		if (Entry->WidgetInstance)
		{
			Entry->WidgetInstance->AddToViewport(Entry->ZOrder);
		}
	}
	return Entry->WidgetInstance.Get();
}