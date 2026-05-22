// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/StageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Gamemode/StageManagerSubsystem.h"
#include "Gamemode/CLGameInstance.h"
#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Enemy/BaseEnemy.h"


// 게임 승리
// Level에 배치된 단일 적 가정
void AStageGameModeBase::OnStageClear()
{
    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        GI->CurrentStatus = ECheckStageResult::Win;
        GI->AddWonBattle();
    }

    if (ABasePlayerController* PC = GetWorld()->GetFirstPlayerController<ABasePlayerController>())
    {
        PC->ShowVictoryUI();
    }


    GetWorldTimerManager().SetTimer(ReturnToLobbyHandle, [this]()
    {
        SaveInventoryToGameInstance();

        UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Level/L_LobbyMap"));
        //UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/L_UITestMap"));
    }, 10.f, false);
}

// 플레이어 한명, 사망 가정
// 게임 오버,
void AStageGameModeBase::OnGameOver()
{
    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        GI->CurrentStatus = ECheckStageResult::Defeat;
    }


    UE_LOG(LogTemp, Warning, TEXT("Player Died"));

    if (ABasePlayerController* PC = GetWorld()->GetFirstPlayerController<ABasePlayerController>())
    {
        PC->ShowDeathUI();
    }

    GetWorldTimerManager().SetTimer(ReturnToLobbyHandle, [this]()
    {
        SaveInventoryToGameInstance();

        UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Level/L_LobbyMap"));
        //UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/L_UITestMap"));
    }, 5.f, false);
}

void AStageGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        GI->CurrentStatus = ECheckStageResult::NotEnd;
        UE_LOG(LogTemp, Warning, TEXT("[GameStage] WonBattleCount: %d"), GI->GetWonBattleCount());
    }

    if (UStageManagerSubsystem* StageSub = GetWorld()->GetSubsystem<UStageManagerSubsystem>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Subsystem binding completed"));
        StageSub->OnAllEnemiesDead.AddDynamic(this, &AStageGameModeBase::OnStageClear);
        StageSub->OnPlayerDead.AddDynamic(this, &AStageGameModeBase::OnGameOver);

        if (DummyEnemyClass && DummyPoolSize > 0)
        {
            TArray<AActor*> SpawnPointActors;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("DummySpawn"), SpawnPointActors);

            TArray<FTransform> SpawnTransforms;
            for (AActor* Actor : SpawnPointActors)
            {
                if (Actor)
                {
                    SpawnTransforms.Add(Actor->GetActorTransform());
                }
            }

            if (SpawnTransforms.Num() > 0)
            {
                StageSub->InitDummyPool(DummyEnemyClass, DummyPoolSize, SpawnTransforms, DummySpawnRadius);
            }
        }
    }
}


void AStageGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);    
}

void AStageGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    LoadInventoryFromGameInstance(NewPlayer);
}



void AStageGameModeBase::SaveInventoryToGameInstance()
{
    UE_LOG(LogTemp, Warning, TEXT("SaveInventory 호출됨"));


    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    if (!GI) return;

    APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (!Player || !Player->ConsumableInventory) return;

    UDataTable* DT = Player->ConsumableInventory->ItemDataTable;
    if (!DT) return;

    TArray<FInventorySlot> Result;

    // 재료는 Stage에서 건드리지 않으므로 GameInstance에서 그대로 보존
    for (const FInventorySlot& Slot : GI->GetSavedInventory())
    {
        if (Slot.IsEmpty())
        {
            continue;
        }
        FItemTableRow* Row = DT->FindRow<FItemTableRow>(Slot.ItemID, TEXT(""));
        if (!Row)
        {
            continue;
        }
        if (Row->ItemType != EItemType::Consumable && Row->ItemType != EItemType::SocketItem)
        {
            Result.Add(Slot);
        }
    }

    // 현재 Stage 인벤토리 추가
    if (Player->ConsumableInventory)
    {
        Result.Append(Player->ConsumableInventory->GetAllItems());
    }
    if (Player->SocketItemInventory)
    {
        Result.Append(Player->SocketItemInventory->GetAllItems());
    }

    GI->SaveInventory(Result);
}

void AStageGameModeBase::LoadInventoryFromGameInstance(APlayerController* NewPlayer)
{
    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    if (!GI) return;

    APlayerCharacter* Player = Cast<APlayerCharacter>(NewPlayer->GetPawn());
    if (!Player || !Player->ConsumableInventory) return;

    UDataTable* DT = Player->ConsumableInventory->ItemDataTable;
    if (!DT) return;

    for (const FInventorySlot& Slot : GI->GetSavedInventory())
    {
        if (Slot.IsEmpty())
        {
            continue;
        }
        FItemTableRow* Row = DT->FindRow<FItemTableRow>(Slot.ItemID, TEXT(""));
        if (!Row)
        {
            continue;
        }

        if (Row->ItemType == EItemType::Consumable && Player->ConsumableInventory)
        {
            Player->ConsumableInventory->AddItem(Slot.ItemID, Slot.Quantity);
        }
        else if (Row->ItemType == EItemType::SocketItem && Player->SocketItemInventory)
        {
            Player->SocketItemInventory->AddItem(Slot.ItemID, Slot.Quantity);
        }
    }
}

