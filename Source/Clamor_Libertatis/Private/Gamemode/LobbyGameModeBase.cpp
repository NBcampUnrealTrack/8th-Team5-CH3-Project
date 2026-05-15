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
    // TODO:: 기획상 LoreManagerComp 완전히 제거 됨.
    // 시나리오 시스템 도입 후 삭제
    
    /*LoreManagerComp = CreateDefaultSubobject<ULoreManagerComponent>(TEXT("LoreManagerComp"));
    
    if (LoreManagerComp) {
        UE_LOG(LogTemp, Warning, TEXT("LoreManager initialized"));
    }*/

    ScenarioManagerComp = CreateDefaultSubobject<UScenarioManagerComponent>(TEXT("ScenarioManagerComp"));
}

void ALobbyGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
    {
        
        // TODO:: 임시코드, 오프닝 1회만 보여줌
        // 시나리오 진행도 작성되면 그거랑 통합하기
        if(GI->bHasSeenOpening == false)
        {
            if (OpeningWidgetClass)
            {
                // 위젯 생성
                UUserWidget* CurrentOpeningWidget = CreateWidget<UUserWidget>(GetWorld(), OpeningWidgetClass);

                if (CurrentOpeningWidget)
                {
                    // 화면에 표시
                    CurrentOpeningWidget->AddToViewport(99);

                    GI->bHasSeenOpening = true;
                }



                UE_LOG(LogTemp, Warning, TEXT("[Lobby] Successed Create Opening Widget"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[Lobby] Failed Create Opening Widget"));
            }
            
            
            GI->bHasSeenOpening = true;
        }

        // TODO:: GameInstance에서 로비에 적용할 적절한 시나리오 Name 가져오기
        if (ScenarioManagerComp)
        {
            ScenarioManagerComp->StartScenario(FName("Scenario_1"));
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


// TODO:: 시나리오 관리 작성 후 완전히 삭제 예정
/*void ALobbyGameModeBase::LookAround()
{
    UE_LOG(LogTemp, Warning, TEXT("Called Look Around"));

    if (LoreManagerComp)
    {
        FText SelectedLore = LoreManagerComp->GetRandomLoreText(0);
        UE_LOG(LogTemp, Warning, TEXT("Selected Lore: %s"), *SelectedLore.ToString());
    }
}*/

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
