// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/StageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Gamemode/StageManagerSubsystem.h"
#include "Gamemode/CLGameInstance.h"
#include "Character/BasePlayerController.h"


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

    UE_LOG(LogTemp, Warning, TEXT("Stage Win"));


    if (ABasePlayerController* PC = GetWorld()->GetFirstPlayerController<ABasePlayerController>())
    {
        PC->ShowVictoryUI();
    }

    // TODO:: 보상처리

    GetWorldTimerManager().SetTimer(ReturnToLobbyHandle, [this]()
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Level/L_LobbyMap"));
    }, 5.f, false);
}

// 플레이어 한명, 사망 가정
// 게임 오버,
void AStageGameModeBase::OnGameOver()
{
    CurrentStatus = ECheckStageResult::Defeat;
    UE_LOG(LogTemp, Warning, TEXT("Player Died"));

    if (ABasePlayerController* PC = GetWorld()->GetFirstPlayerController<ABasePlayerController>())
    {
        PC->ShowDeathUI();
    }

    GetWorldTimerManager().SetTimer(ReturnToLobbyHandle, [this]()
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Level/L_LobbyMap"));
    }, 5.f, false);
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
        UE_LOG(LogTemp, Warning, TEXT("Subsystem binding completed"));
        StageSub->OnAllEnemiesDead.AddDynamic(this, &AStageGameModeBase::OnStageClear);
        StageSub->OnPlayerDead.AddDynamic(this, &AStageGameModeBase::OnGameOver);
    }
}

void AStageGameModeBase::HandleStageResult()
{
    // 적 캐릭터 사망 조건 확인
    // 사망 시, 게임 승리로 간주 후 이동

    // 플레이어 사망 조건 확인
    // 사망시 게임 오버
}

