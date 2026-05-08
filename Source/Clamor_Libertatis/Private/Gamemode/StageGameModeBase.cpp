// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/StageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Gamemode/StageManagerSubsystem.h"
#include "Gamemode/CLGameInstance.h"


// 게임 승리
// Level에 배치된 단일 적 가정
void AStageGameModeBase::OnStageClear()
{
    CurrentStatus = ECheckStageResult::Win;

    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        GI->AddWonBattle();
        UE_LOG(LogTemp, Warning, TEXT("[Stage Clear] WonBattleCount: %d"), GI->GetWonBattleCount());
    }
}

// 플레이어 한명, 사망 가정
// 게임 오버,
void AStageGameModeBase::OnGameOver()
{
    CurrentStatus = ECheckStageResult::Defeat;
    UE_LOG(LogTemp, Warning, TEXT("Player Died"));
}

void AStageGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    CurrentStatus = ECheckStageResult::NotEnd;

    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameStage] WonBattleCount: %d"), GI->GetWonBattleCount());
    }

    if (UStageManagerSubsystem* StageSub = GetWorld()->GetSubsystem<UStageManagerSubsystem>())
    {
        StageSub->OnAllEnemiesDead.AddDynamic(this, &AStageGameModeBase::OnStageClear);
        StageSub->OnPlayerDead.AddDynamic(this, &AStageGameModeBase::OnGameOver);
    }

    // TODO:: 플레이어 캐릭터에 UHealthComponent 사용시 반영
    /** 
    if (AActor* Player = UGameplayStatics::GetActorOfClass(GetWorld(), ::StaticClass()))
    {
        if (UHealthComponent* HC = Player->GetComponentByClass<UHealthComponent>())
        {
            HC->OnDeath.AddDynamic(this, &AStageGameModeBase::OnGameOver);
        }
    }
    **/
}

void AStageGameModeBase::HandleStageResult()
{
    // 적 캐릭터 사망 조건 확인
    // 사망 시, 게임 승리로 간주 후 이동

    // 플레이어 사망 조건 확인
    // 사망시 게임 오버
}

