#include "Gamemode/StageManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Combat/HealthComponent.h"
#include "Character/PlayerCharacter.h"

UStageManagerSubsystem::UStageManagerSubsystem() : RemainingBossCount(0)
{

}

bool UStageManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer)) return false;

	UWorld* World = Cast<UWorld>(Outer);
	if (!World) return false;

	AWorldSettings* WS = World->GetWorldSettings();
	return WS && WS->DefaultGameMode
		&& WS->DefaultGameMode->IsChildOf(AStageGameModeBase::StaticClass());
}

void UStageManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(&InWorld, ABaseEnemy::StaticClass(), Enemies);

	RemainingBossCount = 0;
	for (AActor* Actor : Enemies)
	{
		if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Actor))
		{
			if (!Enemy->ActorHasTag(FName("Boss")))
			{
				continue;
			}
			RemainingBossCount++;
			if (UEnemy_StatComponent* StatComp = Enemy->GetEnemyStatComp())
			{
				StatComp->OnHPChanged.AddDynamic(this, &UStageManagerSubsystem::OnBossHPChanged);
			}
		}
	}

	if (AActor* Player = UGameplayStatics::GetActorOfClass(&InWorld, APlayerCharacter::StaticClass()))
	{
		if (UHealthComponent* HC = Player->GetComponentByClass<UHealthComponent>())
		{
			HC->OnDeath.AddDynamic(this, &UStageManagerSubsystem::OnPlayerDeath);
		}
	}
}

void UStageManagerSubsystem::OnBossHPChanged(float CurrentHP, float MaxHP)
{
	if (CurrentHP <= 0.f)
	{
		RemainingBossCount--;
		if (RemainingBossCount <= 0)
		{
			OnAllEnemiesDead.Broadcast();
		}
	}
}

void UStageManagerSubsystem::OnPlayerDeath()
{
	OnPlayerDead.Broadcast();
}

FVector UStageManagerSubsystem::PickSpawnLocation(const TArray<FVector>& PlacedLocations) const
{
	constexpr int32 Attempts = 8;
	FVector BestLocation = FVector::ZeroVector;
	float BestMinDist = -1.f;

	const int32 BaseIdx = FMath::RandRange(0, DummySpawnTransforms.Num() - 1);

	for (int32 Attempt = 0; Attempt < Attempts; Attempt++)
	{
		const FVector& Base = DummySpawnTransforms[FMath::RandRange(0, DummySpawnTransforms.Num() - 1)].GetLocation();
		const float Angle = FMath::FRandRange(0.f, 360.f);
		const float Dist = FMath::FRandRange(0.f, DummySpawnRadius);
		FVector Candidate = Base + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Dist,
		                                   FMath::Sin(FMath::DegreesToRadians(Angle)) * Dist,
		                                   0.f);

		float MinDist = MAX_FLT;
		for (const FVector& Placed : PlacedLocations)
		{
			MinDist = FMath::Min(MinDist, FVector::DistXY(Candidate, Placed));
		}
		if (PlacedLocations.IsEmpty()) { MinDist = 0.f; }

		if (MinDist > BestMinDist)
		{
			BestMinDist = MinDist;
			BestLocation = Candidate;
		}
	}

	return BestLocation;
}

void UStageManagerSubsystem::InitDummyPool(TSubclassOf<ABaseEnemy> EnemyClass, int32 PoolSize, const TArray<FTransform>& SpawnTransforms, float SpawnRadius)
{
	if (!EnemyClass || PoolSize <= 0 || SpawnTransforms.IsEmpty())
	{
		return;
	}

	DummyEnemyClass = EnemyClass;
	DummySpawnTransforms = SpawnTransforms;
	DummySpawnRadius = SpawnRadius;

	UWorld* World = GetWorld();
	if (!World) { return; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray<FVector> PlacedLocations;
	for (int32 i = 0; i < PoolSize; i++)
	{
		FVector SpawnLoc = PickSpawnLocation(PlacedLocations);
		FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLoc);

		ABaseEnemy* Enemy = World->SpawnActor<ABaseEnemy>(EnemyClass, SpawnTransform, Params);
		if (Enemy)
		{
			PlacedLocations.Add(Enemy->GetActorLocation());
			BindDummyDeath(Enemy);
			DummyPool.Add(Enemy);
		}
	}
}

void UStageManagerSubsystem::BindDummyDeath(ABaseEnemy* Enemy)
{
	if (!Enemy) { return; }
	if (UEnemy_StatComponent* StatComp = Enemy->GetEnemyStatComp())
	{
		StatComp->OnHPChanged.AddDynamic(this, &UStageManagerSubsystem::OnDummyHPChanged);
	}
}

void UStageManagerSubsystem::OnDummyHPChanged(float CurrentHP, float MaxHP)
{
	if (CurrentHP > 0.f) { return; }

	UWorld* World = GetWorld();
	if (!World) { return; }

	for (TObjectPtr<ABaseEnemy>& EnemyPtr : DummyPool)
	{
		ABaseEnemy* Enemy = EnemyPtr.Get();
		if (!Enemy || !Enemy->IsDead() || PendingRespawnDummies.Contains(Enemy))
		{
			continue;
		}

		PendingRespawnDummies.Add(Enemy);
		Enemy->SetLifeSpan(0.f);

		TWeakObjectPtr<ABaseEnemy> WeakEnemy(Enemy);
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle, [this, WeakEnemy]()
		{
			if (ABaseEnemy* Resolved = WeakEnemy.Get())
			{
				PendingRespawnDummies.Remove(Resolved);
				RespawnDummy(Resolved);
			}
		}, 4.f, false);
	}
}

void UStageManagerSubsystem::RespawnDummy(ABaseEnemy* OldEnemy)
{
	if (!DummyEnemyClass || DummySpawnTransforms.IsEmpty()) { return; }

	UWorld* World = GetWorld();
	if (!World) { return; }

	int32 OldIdx = INDEX_NONE;
	for (int32 i = 0; i < DummyPool.Num(); i++)
	{
		if (DummyPool[i].Get() == OldEnemy)
		{
			OldIdx = i;
			break;
		}
	}

	if (OldIdx == INDEX_NONE) { return; }

	OldEnemy->Destroy();
	DummyPool[OldIdx] = nullptr;

	TArray<FVector> PlacedLocations;
	for (const TObjectPtr<ABaseEnemy>& EnemyPtr : DummyPool)
	{
		if (EnemyPtr.Get() && !EnemyPtr->IsDead())
		{
			PlacedLocations.Add(EnemyPtr->GetActorLocation());
		}
	}

	FVector SpawnLoc = PickSpawnLocation(PlacedLocations);
	FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLoc);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseEnemy* NewEnemy = World->SpawnActor<ABaseEnemy>(DummyEnemyClass, SpawnTransform, Params);
	if (NewEnemy)
	{
		BindDummyDeath(NewEnemy);
		DummyPool[OldIdx] = NewEnemy;
	}
}
