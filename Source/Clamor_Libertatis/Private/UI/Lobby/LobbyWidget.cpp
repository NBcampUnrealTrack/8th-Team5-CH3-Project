#include "UI/Lobby/LobbyWidget.h"
#include "Character/BasePlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Crafting/W_CraftingWindow.h"

void ULobbyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);

    if (Btn_StartBattle)
        Btn_StartBattle->OnClicked.AddDynamic(
            this, &ULobbyWidget::OnStartClicked);

    if (StatButton)
        StatButton->OnClicked.AddDynamic(
            this, &ULobbyWidget::OnStatButtonClicked);

    if (CraftingButton) 
        CraftingButton->OnClicked.AddDynamic(
            this, &ULobbyWidget::OnCraftingButtonClicked);
}

void ULobbyWidget::InitLobby(int32 WonBattleCount)
{
    if (Text_WonCount)
        Text_WonCount->SetText(
            FText::FromString(FString::FromInt(WonBattleCount)));

    if (Text_NextBattle)
        Text_NextBattle->SetText(
            FText::FromString(FString::FromInt(WonBattleCount + 1)));

    const TArray<FString> Messages = {
        TEXT("첫 번째 시련이 기다리고 있다."),
        TEXT("두 번째 시련이 기다리고 있다."),
        TEXT("마지막 시련이 기다리고 있다."),
    };

    if (Text_SubMessage)
    {
        int32 Idx = FMath::Clamp(WonBattleCount, 0, Messages.Num() - 1);
        Text_SubMessage->SetText(FText::FromString(Messages[Idx]));
    }

    if (Anim_FadeIn) PlayAnimation(Anim_FadeIn);
}

void ULobbyWidget::OnStartClicked()
{
    OnStartBattleClicked.Broadcast();
}

void ULobbyWidget::OnStatButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("StatButton 클릭됨"));

    ABasePlayerController* PC =
        Cast<ABasePlayerController>(GetOwningPlayer());
    UE_LOG(LogTemp, Warning, TEXT("PC 캐스트: %s"), PC ? TEXT("성공") : TEXT("실패"));

    if (!PC) return;
    PC->ShowMasterInventory();
}



void ULobbyWidget::OnCraftingButtonClicked() 
{
    if (!CraftingWindowClass) return;

    UW_CraftingWindow* CraftingWindow = 
        CreateWidget<UW_CraftingWindow>(GetOwningPlayer(), CraftingWindowClass);
    if (CraftingWindow)
    {
        UE_LOG(LogTemp, Warning, TEXT("CraftingWindow 생성 성공, Viewport 추가"));
        CraftingWindow->AddToViewport(10);
        UE_LOG(LogTemp, Warning, TEXT("IsInViewport: %s"), CraftingWindow->IsInViewport() ? TEXT("true") : TEXT("false"));
    }
}