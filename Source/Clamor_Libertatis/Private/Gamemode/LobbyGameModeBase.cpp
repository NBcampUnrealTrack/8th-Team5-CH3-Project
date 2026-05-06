// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/LobbyGameModeBase.h"
#include "Gamemode/LoreManagerComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameModeBase::ALobbyGameModeBase()
{
    LoreManagerComp = CreateDefaultSubobject<ULoreManagerComponent>(TEXT("LoreManagerComp"));
    
    if (LoreManagerComp) {
        UE_LOG(LogTemp, Warning, TEXT("LoreManager initialized"));
    }
}

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

    if (LoreManagerComp)
    {
        FText SelectedLore = LoreManagerComp->GetRandomLoreText(0);
        UE_LOG(LogTemp, Warning, TEXT("Selected Lore: %s"), *SelectedLore.ToString());
    }
}

void ALobbyGameModeBase::GotoBattle()
{
    // TODO:: 고쳐야 할 코드
    // Battle 장소로 이동할 때, 연출과 동작 방식 등을 아직 결정하지 못함.
    // 지연 로딩을 통해 느낌만 부여함.



    FTimerHandle BattleTimerHandle;
}
