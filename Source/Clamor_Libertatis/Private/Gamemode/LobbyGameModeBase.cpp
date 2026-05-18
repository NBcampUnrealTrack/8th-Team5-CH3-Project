// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/LobbyGameModeBase.h"
// TODO:: 더이상 쓰지 않음. 삭제 해야함
// #include "Gamemode/LoreManagerComponent.h"
#include "Gamemode/CLGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "Gamemode/Scenario/ScenarioManagerComponent.h"


ALobbyGameModeBase::ALobbyGameModeBase()
{
    ScenarioManagerComp = CreateDefaultSubobject<UScenarioManagerComponent>(TEXT("ScenarioManagerComp"));
    
}

void ALobbyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        
        if (ScenarioManagerComp)
        {
           

            if (!GI->HasWatchedOpening())
            {
                ScenarioManagerComp->StartScenario(GI->LastScenarioRowName);
            }
            else
            {
                ScenarioManagerComp->StartScenario("Question_0");
            }
                       
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[Lobby] ScenarioManagerComp not found"));
        }
    } 
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Lobby] Failed to get GameInstance"));
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();

    if (PC)
    {
        PC->bShowMouseCursor = true;
        PC->bEnableClickEvents = true;
        PC->bEnableMouseOverEvents = true;

        FInputModeUIOnly InputModeData;
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputModeData);
    }


}

void ALobbyGameModeBase::ReadyComplete()
{
    if (!ScenarioManagerComp->IsScenarioEnd())
    {
        return;
    }

    // 로비관련 UI 비활성화
    UE_LOG(LogTemp, Warning, TEXT("Called Ready Complete"));

    APlayerController* PC = GetWorld()->GetFirstPlayerController();

    if (PC)
    {
        PC->bShowMouseCursor = false;
        FInputModeGameOnly InputModeData;
        InputModeData.SetConsumeCaptureMouseDown(true);
        PC->SetInputMode(InputModeData);
    }



    // 연출 후에, 
    // Room Level 이 구현되지 않은 경우 바로 BattleMap으로 이동
    GotoBattle();
    // Room Level 이 구축된 경우, 포탈과의 상호작용을 통해 이동    
}



void ALobbyGameModeBase::GotoBattle()
{
    // TODO:: 고쳐야 할 코드
    // Battle 장소로 이동할 때, 연출과 동작 방식 등을 아직 결정하지 못함.
    // 지연 로딩을 통해 느낌만 부여함.

    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        //UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Level/L_MainStage"));

        int32 count = GI->GetWonBattleCount();

        if (count == 0)
        {
            UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Level/L_Stage1"));
        }
        else if (count == 1)
        {
            UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Level/L_Stage2"));
        }
        else if (count == 2)
        {
            UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Level/L_Stage3"));
        }
        else
        {
            UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Level/L_Stage3"));
        }
    
        UE_LOG(LogTemp, Warning, TEXT("[Lobby] Current Won Battle Count: %d"), GI->GetWonBattleCount());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[Lobby] Failed to get GameInstance"));
    }

    
}
