// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/StageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Enemy/BaseEnemy.h"
#include "Combat/HealthComponent.h"
#include "Combat/TestCharacter.h"


// 게임 승리
// Level에 배치된 단일 적 가정
void AStageGameModeBase::OnEnemyDeath()
{
    CurrentStatus = ECheckStageResult::Win;
}

// 플레이어 한명, 사망 가정
// 게임 오버,
void AStageGameModeBase::OnPlayerDeath()
{
    CurrentStatus = ECheckStageResult::Defeat;
}

void AStageGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    CachedGameState = GetGameState<AGameStateBase>();
    CurrentStatus = ECheckStageResult::NotEnd;

    // 런타임 Spawn시 별도로 바인딩이 필요함.
    TArray<AActor*> Enemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), Enemies);
    for (AActor* Actor : Enemies)
    {
        if (UHealthComponent* HC = Actor->GetComponentByClass<UHealthComponent>())
        {
            HC->OnDeath.AddDynamic(this, &AStageGameModeBase::OnEnemyDeath);
        }
    }

    // TODO:: TestCharacter가 아니라 플레이어 캐릭터로 교체
    if (AActor* Player = UGameplayStatics::GetActorOfClass(GetWorld(), ATestCharacter::StaticClass()))
    {
        if (UHealthComponent* HC = Player->GetComponentByClass<UHealthComponent>())
        {
            HC->OnDeath.AddDynamic(this, &AStageGameModeBase::OnPlayerDeath);
        }
    }
}

// Binding
void AStageGameModeBase::HandleStageResult()
{
    // 적 캐릭터 사망 조건 확인
    // 사망 시, 게임 승리로 간주 후 이동

    // 플레이어 사망 조건 확인
    // 사망시 게임 오버
}

