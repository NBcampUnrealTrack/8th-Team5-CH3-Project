#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Combat/HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/UIManager.h"
#include "UI/EnemyTrackerComponent.h"
#include "Combat/SkillComponent.h"

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

void ABasePlayerController::ShowLobbyUI()
{
	if (UIManager) UIManager->ShowWidget(EUIType::Lobby);
}
void ABasePlayerController::HideLobbyUI()
{
	if (UIManager) UIManager->HideWidget(EUIType::Lobby);
}
void ABasePlayerController::ShowDeathUI()
{
	if (UIManager) UIManager->ShowWidget(EUIType::Death);
}
void ABasePlayerController::HideDeathUI()
{
	if (UIManager) UIManager->HideWidget(EUIType::Death);
}
void ABasePlayerController::ShowVictoryUI()
{
	if (UIManager) UIManager->ShowWidget(EUIType::Victory);
}
void ABasePlayerController::HideVictoryUI()
{
	if (UIManager) UIManager->HideWidget(EUIType::Victory);
}

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

	HUDWidgetRef->OnHealthChanged(HealthComp->GetCurrentHealth(), HealthComp->GetMaxHealth());
	HUDWidgetRef->OnStaminaChanged(HealthComp->GetCurrentStamina(), HealthComp->GetMaxStamina());

	HealthComp->OnHealthChanged.AddDynamic(HUDWidgetRef, &UPlayerHUDWidget::OnHealthChanged);
	HealthComp->OnStaminaChanged.AddDynamic(HUDWidgetRef, &UPlayerHUDWidget::OnStaminaChanged);

	USkillComponent* SkillComp = PlayerCharacter->FindComponentByClass<USkillComponent>();
	if (SkillComp)
	{
		HUDWidgetRef->InitSkillCooldown(SkillComp);
	}

}
