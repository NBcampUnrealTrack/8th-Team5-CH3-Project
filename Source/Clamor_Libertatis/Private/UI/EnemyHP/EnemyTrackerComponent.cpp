#include "UI/EnemyHP/EnemyTrackerComponent.h"
#include "Enemy/BaseEnemy.h"
#include "UI/EnemyHP/EnemyHPBarWidget.h"
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

	float ClosestDistSq = FMath::Square(ShowDistance);
	float BestDot = -1.f;

	const FVector MyLoc = MyPawn->GetActorLocation();
	const FVector Forward = MyPawn->GetActorForwardVector();

	for (TActorIterator<ABaseEnemy> It(OwnerController->GetWorld()); It; ++It)
	{
		ABaseEnemy* Enemy = *It;

		if (!Enemy || Enemy->IsDead())
		{
			continue;
		}

		const FVector EnemyLoc = Enemy->GetActorLocation();
		const float DistSq = FVector::DistSquared(MyLoc, EnemyLoc);

		if (DistSq > FMath::Square(ShowDistance))
		{
			continue;
		}

		const FVector Dir = (EnemyLoc - MyLoc).GetSafeNormal();
		const float Dot = FVector::DotProduct(Forward, Dir);

		bool bShouldReplace = false;

		if (DistSq < ClosestDistSq)
		{
			bShouldReplace = true;
		}
		
		else if (FMath::IsNearlyEqual(DistSq, ClosestDistSq, 1.f))
		{
			if (Dot > BestDot)
			{
				bShouldReplace = true;
			}
		}

		if (bShouldReplace)
		{
			ClosestDistSq = DistSq;
			BestDot = Dot;
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