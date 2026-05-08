#include "UI/EnemyTrackerComponent.h"
#include "Enemy/BaseEnemy.h"
#include "UI/EnemyHPBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h"
#include "TimerManager.h"

void UEnemyTrackerComponent::Initialize(APlayerController* InOwner)
{
	OwnerController = InOwner;
}

void UEnemyTrackerComponent::StartTracking()
{
	if (!OwnerController) return;

	if (UEnemyHPBarWidget* W = GetOrCreateHPBarWidget())
	{
		W->SetVisibility(ESlateVisibility::Hidden);
	}

	OwnerController->GetWorldTimerManager().SetTimer(
		UpdateTimerHandle,
		this,
		&UEnemyTrackerComponent::Tick,
		UpdateInterval,
		true
	);
}

void UEnemyTrackerComponent::StopTracking()
{
	if (OwnerController)
	{
		OwnerController->GetWorldTimerManager().ClearTimer(UpdateTimerHandle);
	}
}

void UEnemyTrackerComponent::SetSuppressed(bool bSuppress)
{
	bSuppressed = bSuppress;
	if (bSuppressed && HPBarWidget)
	{
		HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEnemyTrackerComponent::Tick()
{
	if (bSuppressed)
	{
		if (HPBarWidget) HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	ABaseEnemy* Closest = FindClosestEnemy();
	if (!Closest)
	{
		if (HPBarWidget) HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
		TrackedEnemy = nullptr;
		return;
	}

	UEnemyHPBarWidget* W = GetOrCreateHPBarWidget();
	if (!W) return;

	if (TrackedEnemy != Closest)
	{
		TrackedEnemy = Closest;
		W->SetTargetEnemy(Closest);
	}

	if (!W->IsVisible())
	{
		W->SetVisibility(ESlateVisibility::Visible);
	}
}

ABaseEnemy* UEnemyTrackerComponent::FindClosestEnemy() const
{
	if (!OwnerController) return nullptr;

	APawn* MyPawn = OwnerController->GetPawn();
	if (!MyPawn) return nullptr;

	ABaseEnemy* Closest = nullptr;
	float       ClosestDistSq = FMath::Square(ShowDistance);
	FVector     MyLoc = MyPawn->GetActorLocation();

	for (TActorIterator<ABaseEnemy> It(OwnerController->GetWorld()); It; ++It)
	{
		ABaseEnemy* Enemy = *It;
		if (!Enemy || Enemy->IsDead()) continue;

		const float DistSq = FVector::DistSquared(MyLoc, Enemy->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			Closest = Enemy;
		}
	}
	return Closest;
}

UEnemyHPBarWidget* UEnemyTrackerComponent::GetOrCreateHPBarWidget()
{
	if (!HPBarWidget && HPBarWidgetClass && OwnerController)
	{
		HPBarWidget = CreateWidget<UEnemyHPBarWidget>(OwnerController.Get(), HPBarWidgetClass);
		if (HPBarWidget)
		{
			HPBarWidget->AddToViewport(0);
		}
	}
	return HPBarWidget.Get();
}