#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Combat/HealthComponent.h"
#include "EnhancedInputSubsystems.h"

//UI
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/UIManager.h"
#include "UI/EnemyHP/EnemyTrackerComponent.h"
#include "Combat/SkillComponent.h"
#include "UI/Inventory/QuickSlotWidget.h"
#include "UI/Inventory/InventoryWidget.h"
#include "Item/Inventory/ConsumableInventoryComponent.h"
#include "UI/Inventory/StatWidget.h"

ABasePlayerController::ABasePlayerController()
	: InputMappingContext(nullptr)
	, MoveAction(nullptr)
	, JumpAction(nullptr)
	, LookAction(nullptr)
	, SprintAction(nullptr)
	, BasicAttackAction(nullptr)
	, DodgeAction(nullptr)
	, ActiveSkillAction(nullptr)
	, LockAction(nullptr)
	, UIManager(nullptr)
	, EnemyTracker(nullptr)
	, HUDWidgetRef(nullptr)
{
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeInput();

	if (UIManager)
	{
		UIManager->Initialize(this);
	}

	if (EnemyTracker)
	{
		EnemyTracker->Initialize(this);
		EnemyTracker->StartTracking();
	}

	ShowGameStartUI();
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(EKeys::One, IE_Pressed, this, &ABasePlayerController::UseQuickSlot1);
	InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ABasePlayerController::UseQuickSlot2);
	InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ABasePlayerController::UseQuickSlot3);
	InputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ABasePlayerController::UseQuickSlot4);
}

void ABasePlayerController::InitializeInput()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer) return;

	auto* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (Subsystem && InputMappingContext)
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
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
	if (EnemyTracker) EnemyTracker->SetSuppressed(false);
	
	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
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
	SetInputMode(FInputModeUIOnly{});
	bShowMouseCursor = true;
}

void ABasePlayerController::ContinueGame()
{
	if (!UIManager) return;
	UIManager->HideWidget(EUIType::MainMenu);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
}

void ABasePlayerController::RestartGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ABasePlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}

void ABasePlayerController::ShowInventory()
{
	if (!UIManager) return;

	UUserWidget* RawWidget = UIManager->GetOrCreateWidget(EUIType::Inventory);
	UE_LOG(LogTemp, Log, TEXT("RawWidget: %s"), RawWidget ? TEXT("있음") : TEXT("없음"));

	UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(RawWidget);
	UE_LOG(LogTemp, Log, TEXT("InventoryWidget 캐스트: %s"), InventoryWidget ? TEXT("성공") : TEXT("실패"));

	if (!InventoryWidget) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	UE_LOG(LogTemp, Log, TEXT("PlayerCharacter: %s"), PlayerCharacter ? TEXT("있음") : TEXT("없음"));
	if (!PlayerCharacter) return;

	InventoryWidget->InitInventory(
		PlayerCharacter->ConsumableInventory,
		QuickSlotWidgetRef
	);

	UIManager->ShowWidget(EUIType::Inventory);

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SetInputMode(FInputModeUIOnly{});
	bShowMouseCursor = true;
}

void ABasePlayerController::HideInventory()
{
	if (!UIManager) return;
	UIManager->HideWidget(EUIType::Inventory);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
}

void ABasePlayerController::ShowStatWidget()
{
	if (!UIManager) return;

	UUserWidget* RawWidget = UIManager->GetOrCreateWidget(EUIType::Stat);
	UStatWidget* StatWidget = Cast<UStatWidget>(RawWidget);
	if (!StatWidget) return;

	StatWidget->InitStatWidget(this);
	UIManager->ShowWidget(EUIType::Stat);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SetInputMode(FInputModeUIOnly{});
	bShowMouseCursor = true;
}

void ABasePlayerController::HideStatWidget()
{
	if (!UIManager) return;
	UIManager->HideWidget(EUIType::Stat);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
}
void ABasePlayerController::InitQuickSlotWidget()
{
	if (!UIManager) return;

	UIManager->ShowWidget(EUIType::QuickSlot);

	UQuickSlotWidget* QuickSlotWidget =
		Cast<UQuickSlotWidget>(UIManager->GetWidget(EUIType::QuickSlot));

	UE_LOG(LogTemp, Log, TEXT("QuickSlotWidget 캐스트: %s"),
		QuickSlotWidget ? TEXT("성공") : TEXT("실패"));

	if (!QuickSlotWidget) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!PlayerCharacter) return;

	QuickSlotWidget->InitQuickSlot(PlayerCharacter->ConsumableInventory);

	QuickSlotWidgetRef = QuickSlotWidget;

	UE_LOG(LogTemp, Log, TEXT("QuickSlotWidgetRef 설정 완료: %s"),
		QuickSlotWidgetRef ? TEXT("성공") : TEXT("실패"));
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