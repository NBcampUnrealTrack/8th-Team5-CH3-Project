#include "Gamemode/StageManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/BaseEnemy.h"
#include "Combat/HealthComponent.h"
#include "Character/PlayerCharacter.h"

UStageManagerSubsystem::UStageManagerSubsystem() : RemainingEnemyCount(0)
{

}

void UStageManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(&InWorld, ABaseEnemy::StaticClass(), Enemies);
	RemainingEnemyCount = Enemies.Num();
	for (AActor* Actor : Enemies)
	{
		if (UHealthComponent* HC = Actor->GetComponentByClass<UHealthComponent>())
		{
			HC->OnDeath.AddDynamic(this, &UStageManagerSubsystem::OnEnemyDeath);
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

void UStageManagerSubsystem::OnEnemyDeath()
{
	RemainingEnemyCount--;
	if (RemainingEnemyCount <= 0)
	{
		OnAllEnemiesDead.Broadcast();
	}
}

void UStageManagerSubsystem::OnPlayerDeath()
{
	OnPlayerDead.Broadcast();
}
