#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyTrackerComponent.generated.h"

class ABaseEnemy;
class UEnemyHPBarWidget;
class APlayerController;

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class CLAMOR_LIBERTATIS_API UEnemyTrackerComponent : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* InOwner);
	void StartTracking();
	void StopTracking();
	void SetSuppressed(bool bSuppress);

	ABaseEnemy* GetTrackedEnemy() const { return TrackedEnemy.Get(); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Tracker")
	TSubclassOf<UEnemyHPBarWidget> HPBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Tracker")
	float ShowDistance = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Tracker")
	float UpdateInterval = 0.15f;

private:
	UPROPERTY()
	TObjectPtr<APlayerController> OwnerController;

	UPROPERTY()
	TObjectPtr<UEnemyHPBarWidget> HPBarWidget;

	TWeakObjectPtr<ABaseEnemy> TrackedEnemy;

	FTimerHandle UpdateTimerHandle;
	bool bSuppressed = false;

	void Tick();
	ABaseEnemy* FindClosestEnemy() const;
	UEnemyHPBarWidget* GetOrCreateHPBarWidget();
};