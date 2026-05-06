// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/LobbyGameModeBase.h"
#include "GameFramework/PlayerController.h"


void ALobbyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

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

void ALobbyGameModeBase::LobbyLoop()
{
    // 로비관련 UI 활성화
}

void ALobbyGameModeBase::ReadyComplete()
{
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

void ALobbyGameModeBase::LookAround()
{
    UE_LOG(LogTemp, Warning, TEXT("Called Look Around"));
}

void ALobbyGameModeBase::GotoBattle()
{

}
