#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Combat/HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/UIManager.h"
#include "UI/EnemyHP/EnemyTrackerComponent.h"
#include "Combat/SkillComponent.h"
#include "UI/Inventory/QuickSlotWidget.h"
#include "Item/Inventory/ConsumableInventoryComponent.h"
#include "UI/Inventory/MasterInventoryWidget.h"
#include "UI/Opening/OpeningSequencer.h"
#include "UI/Opening/OpeningWidget.h"

#include "Gamemode/CLGameInstance.h"
#include "Gamemode/Scenario/ScenarioManagerComponent.h"
#include "Gamemode/LobbyGameModeBase.h"
#include "Crafting/CraftingComponent.h"

//UI Test용 게임 모드 완성시 삭제
#include "Gamemode/EndingModeBase.h"

ABasePlayerController::ABasePlayerController()
    : InputMappingContext(nullptr)
    , MoveAction(nullptr)
    , JumpAction(nullptr)
    , LookAction(nullptr)
    , SprintAction(nullptr)
    , BasicAttackAction(nullptr)
    , DodgeAction(nullptr)
    , ActiveSkillAction_Q(nullptr)
    , ActiveSkillAction_E(nullptr)
    , ActiveSkillAction_F(nullptr)
    , LockAction(nullptr)
    , UIManager(nullptr)
    , EnemyTracker(nullptr)
    , HUDWidgetRef(nullptr)
    , OpeningSequencer(nullptr)
    , CraftingComp(nullptr)
{
    CraftingComp = CreateDefaultSubobject<UCraftingComponent>(TEXT("CraftingComp"));
}

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();
    InitializeInput();

    if (UIManager)
        UIManager->Initialize(this);

    if (EnemyTracker)
    {
        EnemyTracker->Initialize(this);
        EnemyTracker->StartTracking();
    }

    UCLGameInstance* GI = GetGameInstance<UCLGameInstance>();
    if (!GI || !GI->HasWatchedOpening())
    {
        // 최초 실행만 타이틀 표시
        ShowGameStartUI();
    }
    // 이후 레벨 진입은 GameMode가 처리
    else
    {
        // 이후 레벨 진입
        // 로비 레벨 -> UITextLobbyGameMode가 질문 처리
        // 전투 레벨 -> StartGame으로 HUD 표시
        // 엔딩 레벨 -> UITestEndginGamemode로 엔딩 UI 띄우게
        bool bIsLobby = GetWorld()->GetAuthGameMode<ALobbyGameModeBase>() != nullptr; // 추후 진짜 로비 게임 모드로
        bool bIsEnding = GetWorld()->GetAuthGameMode<AEndingModeBase>() != nullptr; // 추후 진짜 엔딩 게임 모드

        if (!bIsLobby && !bIsEnding)
        {
            // 전투 레벨 -> HUD 표시
            StartGame();

            // 텍스쳐 스트리밍 대기 시간 고려하여 페이드 인 처리
            if (PlayerCameraManager)
            {
                PlayerCameraManager->StartCameraFade(1.0f, 0.0f, 3.0f, FLinearColor::Black, false, false);
            }
        }
    }

}


void ABasePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindKey(EKeys::One, IE_Pressed, this, &ABasePlayerController::UseQuickSlot1);
    InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ABasePlayerController::UseQuickSlot2);
    InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ABasePlayerController::UseQuickSlot3);
    InputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ABasePlayerController::UseQuickSlot4);

    InputComponent->BindKey(EKeys::C, IE_Pressed, this, &ABasePlayerController::ToggleMainMenu);
    InputComponent->BindKey(EKeys::V, IE_Pressed, this, &ABasePlayerController::ToggleMasterInventory);
}


void ABasePlayerController::InitializeInput()
{
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer) return;

    auto* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (Subsystem && InputMappingContext)
        Subsystem->AddMappingContext(InputMappingContext, 0);
}

void ABasePlayerController::SetStageState(ECheckStageResult NewState)
{
    if (!UIManager) return;

    UIManager->HideWidget(EUIType::Death);
    UIManager->HideWidget(EUIType::Victory);

    switch (NewState)
    {
    case ECheckStageResult::NotEnd:
        InitHUDWidget();
        if (HUDWidgetRef) HUDWidgetRef->SetVisibility(ESlateVisibility::Visible);
        break;
    case ECheckStageResult::Win:
        ShowVictoryUI();
        break;
    case ECheckStageResult::Defeat:
        ShowDeathUI();
        break;
    }
}

void ABasePlayerController::ShowGameStartUI()
{
    if (!UIManager) return;
    UIManager->ShowWidget(EUIType::GameStart);
    if (EnemyTracker) EnemyTracker->SetSuppressed(true);
    SetInputMode(FInputModeUIOnly{});
    bShowMouseCursor = true;
}

void ABasePlayerController::HideGameStartUI()
{
    if (!UIManager) return;
    UIManager->HideWidget(EUIType::GameStart);
    
}

void ABasePlayerController::OnStartButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnStartButtonClicked 호출됨"));
    HideGameStartUI();

    if (!UIManager || !OpeningScenarioTable)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManager 또는 OpeningScenarioTable 없음"));
        StartGame();
        return;
    }

    UIManager->ShowWidget(EUIType::Opening);
    if (EnemyTracker) EnemyTracker->SetSuppressed(true);

    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
    SetInputMode(FInputModeUIOnly{});  
    bShowMouseCursor = false;          

    OpeningSequencer = NewObject<UOpeningSequencer>(this);
    OpeningSequencer->ScenarioTable = OpeningScenarioTable;
    OpeningSequencer->OnOpeningEnd.AddDynamic(
        this, &ABasePlayerController::OnOpeningEnd);

    UOpeningWidget* OpeningWidget = Cast<UOpeningWidget>(
        UIManager->GetOrCreateWidget(EUIType::Opening));

    UE_LOG(LogTemp, Warning, TEXT("OpeningWidget 캐스트: %s"),
        OpeningWidget ? TEXT("성공") : TEXT("실패"));

    if (OpeningWidget)
        OpeningWidget->InitWidget(OpeningSequencer);

    OpeningSequencer->StartSequence(FName("Scenario_1"));
}

void ABasePlayerController::OnOpeningEnd()
{
    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
        GI->LastScenarioRowName = FName("Scenario_End");

    if (UIManager) UIManager->HideWidget(EUIType::Opening);

    if (ALobbyGameModeBase* LobbyGM =
        GetWorld()->GetAuthGameMode<ALobbyGameModeBase>())
    {
        bShowMouseCursor = true;
        bEnableClickEvents = true;
        bEnableMouseOverEvents = true;
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);
        LobbyGM->ShowLobbyPhase();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnOpeningEnd: LobbyGameMode 아님 -> StartGame 호출"));
        StartGame();
    }
}

void ABasePlayerController::StartGame()
{
    if (EnemyTracker) EnemyTracker->SetSuppressed(false);
    SetStageState(ECheckStageResult::NotEnd);

    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
    SetInputMode(FInputModeGameOnly{});
}

void ABasePlayerController::ShowLobbyUI() { if (UIManager) UIManager->ShowWidget(EUIType::Lobby); }
void ABasePlayerController::HideLobbyUI() { if (UIManager) UIManager->HideWidget(EUIType::Lobby); }
void ABasePlayerController::ShowDeathUI() { if (UIManager) UIManager->ShowWidget(EUIType::Death); }
void ABasePlayerController::HideDeathUI() { if (UIManager) UIManager->HideWidget(EUIType::Death); }
void ABasePlayerController::ShowVictoryUI() { if (UIManager) UIManager->ShowWidget(EUIType::Victory); }
void ABasePlayerController::HideVictoryUI() { if (UIManager) UIManager->HideWidget(EUIType::Victory); }

void ABasePlayerController::ShowMainMenu()
{
    if (!UIManager) return;
    UIManager->ShowWidget(EUIType::MainMenu);
    UGameplayStatics::SetGamePaused(GetWorld(), true);

    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
    bShowMouseCursor = true;;
}

void ABasePlayerController::ContinueGame()
{
    if (!UIManager) return;
    UIManager->HideWidget(EUIType::MainMenu);
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    if (GetWorld()->GetAuthGameMode<AStageGameModeBase>())
    {
        // 로비 -> UI 모드 유지
        bShowMouseCursor = true;
        bEnableClickEvents = true;
        bEnableMouseOverEvents = true;
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        SetInputMode(InputMode);
    }
    else
    {
        // 전투 -> 게임 모드
        bShowMouseCursor = false;
        bEnableClickEvents = false;
        bEnableMouseOverEvents = false;
        SetInputMode(FInputModeGameOnly{});
    }
}

void ABasePlayerController::RestartGame()
{
    if (UCLGameInstance* GI = GetGameInstance<UCLGameInstance>())
        GI->ResetGame();

    UGameplayStatics::SetGamePaused(GetWorld(), false);
    SetInputMode(FInputModeGameOnly{});
    bShowMouseCursor = false;
    UGameplayStatics::OpenLevel(this, FName("/Game/Level/L_LobbyMap")); //추후 진짜 게임 로비 모드로
}

void ABasePlayerController::QuitGame()
{
    UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}

void ABasePlayerController::GoToLobby()
{
    UGameplayStatics::OpenLevel(this, FName("/Game/Level/L_LobbyMap"));
}

void ABasePlayerController::GoToEnding()
{
    UGameplayStatics::OpenLevel(this, FName("/Game/Level/L_Ending"));
}

void ABasePlayerController::ShowMasterInventory()
{
    if (!UIManager) { UE_LOG(LogTemp, Error, TEXT("UIManager 없음")); return; }

    UUserWidget* RawWidget = UIManager->GetOrCreateWidget(EUIType::MasterInventory);
    UMasterInventoryWidget* MasterWidget = Cast<UMasterInventoryWidget>(RawWidget);
    if (!MasterWidget) return;

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter) return;

    MasterWidget->InitWidget(
        PlayerCharacter->ConsumableInventory,
        PlayerCharacter->SocketItemInventory,
        QuickSlotWidgetRef,
        PlayerCharacter->SpawnedWeapon
    );

    UIManager->ShowWidget(EUIType::MasterInventory);
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
    bShowMouseCursor = true;

    if (!GetWorld()->GetAuthGameMode<ALobbyGameModeBase>()) //추후 진짜 로비 게임 모드로
        UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ABasePlayerController::HideMasterInventory()
{
    if (!UIManager) return;
    UIManager->HideWidget(EUIType::MasterInventory);
    bShowMouseCursor = false;

    // 로비면 UI 모드 유지, 전투면 게임 모드로
    if (GetWorld()->GetAuthGameMode<AStageGameModeBase>()) // 추후 진짜 게임 모드로 변경
    {
        // 로비 -> UI 모드 유지
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        SetInputMode(InputMode);
        bShowMouseCursor = true;
    }
    else
    {
        // 전투 -> 게임 모드로 복귀
        UGameplayStatics::SetGamePaused(GetWorld(), false);
        SetInputMode(FInputModeGameOnly{});
        bShowMouseCursor = false;
    }
}

void ABasePlayerController::InitQuickSlotWidget()
{
    if (!UIManager) return;
    UIManager->ShowWidget(EUIType::QuickSlot);

    UQuickSlotWidget* QuickSlotWidget =
        Cast<UQuickSlotWidget>(UIManager->GetWidget(EUIType::QuickSlot));
    if (!QuickSlotWidget) return;

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter) return;

    QuickSlotWidget->InitQuickSlot(PlayerCharacter->ConsumableInventory);
    QuickSlotWidgetRef = QuickSlotWidget;
}

void ABasePlayerController::InitHUDWidget()
{
    if (HUDWidgetRef || !HUDWidgetClass) return;

    HUDWidgetRef = CreateWidget<UPlayerHUDWidget>(this, HUDWidgetClass);
    if (!HUDWidgetRef) return;

    HUDWidgetRef->AddToViewport(ZOrder_HUD);

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
    if (!PlayerCharacter) return;

    UHealthComponent* HealthComp = PlayerCharacter->GetHealthComponent();
    if (!HealthComp) return;

    HUDWidgetRef->InitWidget(HealthComp);

    USkillComponent* SkillComp = PlayerCharacter->FindComponentByClass<USkillComponent>();
    if (SkillComp)
        HUDWidgetRef->InitSkillCooldown(SkillComp);

    InitQuickSlotWidget();
}

void ABasePlayerController::UseQuickSlot1() { if (QuickSlotWidgetRef) QuickSlotWidgetRef->UseQuickSlot(0); }
void ABasePlayerController::UseQuickSlot2() { if (QuickSlotWidgetRef) QuickSlotWidgetRef->UseQuickSlot(1); }
void ABasePlayerController::UseQuickSlot3() { if (QuickSlotWidgetRef) QuickSlotWidgetRef->UseQuickSlot(2); }
void ABasePlayerController::UseQuickSlot4() { if (QuickSlotWidgetRef) QuickSlotWidgetRef->UseQuickSlot(3); }

void ABasePlayerController::ToggleMasterInventory()
{
    if (!UIManager) return;
    if (UIManager->IsWidgetVisible(EUIType::MasterInventory))
        HideMasterInventory();
    else
        ShowMasterInventory();
}

void ABasePlayerController::ToggleMainMenu()
{
    if (!UIManager) return;
    if (UIManager->IsWidgetVisible(EUIType::MainMenu))
        ContinueGame();
    else
        ShowMainMenu();
}