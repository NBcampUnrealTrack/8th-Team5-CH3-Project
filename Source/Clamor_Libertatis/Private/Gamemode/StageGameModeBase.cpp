// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/StageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"


void AStageGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Player & Enemy 사망 이벤트에 대해서 바인딩

    /**
     *
     *  적 리스트 가져오기
     *
        TArray<AActor*> OutEnemies;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyDummy::StaticClass(), OutEnemies);
     *
     **/

    // 캐릭터 & 적 사망시 승리 & 종료 조건 체크하도록 바인딩 처리.
}

void AStageGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    CachedGameState = GetGameState<AGameStateBase>();
}

// Binding
void AStageGameModeBase::HandleStageResult()
{
    // 적 캐릭터 사망 조건 확인
    // 사망 시, 게임 승리로 간주 후 이동

    // 플레이어 사망 조건 확인
    // 사망시 게임 오버
}

