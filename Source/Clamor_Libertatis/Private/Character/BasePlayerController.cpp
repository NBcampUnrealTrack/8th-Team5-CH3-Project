#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Combat/HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/EnemyHPBarWidget.h"
#include "Enemy/BaseEnemy.h"
#include "EngineUtils.h" 

ABasePlayerController::ABasePlayerController()
	: InputMappingContext(nullptr)
	, MoveAction(nullptr)
	, JumpAction(nullptr)
	, LookAction(nullptr)
	, SprintAction(nullptr)
	, HUDWidgetRef(nullptr)
	, DeathWidgetRef(nullptr)
	, VictoryWidgetRef(nullptr)
	, AttackAction(nullptr)
	, DodgeAction(nullptr)
	, EnemyHPBarWidget(nullptr)
{
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
	ShowGameStartUI();

	GetOrCreateWidget(EnemyHPBarWidget, EnemyHPBarWidgetClass);
	if (EnemyHPBarWidget)
		EnemyHPBarWidget->SetVisibility(ESlateVisibility::Hidden);
	GetWorldTimerManager().SetTimer(HPBarUpdateTimerHandle, this, &ABasePlayerController::UpdateEnemyHPBar, HPBarUpdateInterval, true);
}

void ABasePlayerController::SetStageState(ECheckStageResult NewState)
{
	HideWidgetInternal(DeathWidgetRef);
	HideWidgetInternal(VictoryWidgetRef);

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
	ShowWidgetInternal(GameStartWidgetRef, GameStartWidgetClass);
	SetInputMode(FInputModeUIOnly{});
	bShowMouseCursor = true;
}

void ABasePlayerController::HideGameStartUI()
{
	HideWidgetInternal(GameStartWidgetRef);
	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
}

void ABasePlayerController::ShowLobbyUI() { ShowWidgetInternal(LobbyWidgetRef, LobbyWidgetClass); }
void ABasePlayerController::HideLobbyUI() { HideWidgetInternal(LobbyWidgetRef); }

void ABasePlayerController::ShowDeathUI() { ShowWidgetInternal(DeathWidgetRef, DeathWidgetClass, ZOrder_Death); }
void ABasePlayerController::HideDeathUI() { HideWidgetInternal(DeathWidgetRef); }

void ABasePlayerController::ShowVictoryUI() { ShowWidgetInternal(VictoryWidgetRef, VictoryWidgetClass, ZOrder_Victory); }
void ABasePlayerController::HideVictoryUI() { HideWidgetInternal(VictoryWidgetRef); }

void ABasePlayerController::ShowMainMenu()
{
	ShowWidgetInternal(MainMenuWidgetRef, MainMenuWidgetClass, ZOrder_MainMenu);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	SetInputMode(FInputModeUIOnly{});
	bShowMouseCursor = true;
}

void ABasePlayerController::ContinueGame()
{
	HideWidgetInternal(MainMenuWidgetRef);
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

void ABasePlayerController::ShowWidgetInternal(TObjectPtr<UUserWidget>& WidgetRef,
	TSubclassOf<UUserWidget> WidgetClass,
	int32 ZOrder)
{
	if (auto* W = GetOrCreateWidget(WidgetRef, WidgetClass, ZOrder))
		W->SetVisibility(ESlateVisibility::Visible);
}

void ABasePlayerController::HideWidgetInternal(TObjectPtr<UUserWidget>& WidgetRef)
{
	if (WidgetRef)
		WidgetRef->SetVisibility(ESlateVisibility::Collapsed);
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

	HUDWidgetRef->OnHealthChanged(HealthComp->CurrentHealth, HealthComp->MaxHealth);
	HUDWidgetRef->OnStaminaChanged(HealthComp->CurrentStamina, HealthComp->MaxStamina);

	HealthComp->OnHealthChanged.AddDynamic(HUDWidgetRef, &UPlayerHUDWidget::OnHealthChanged);
	HealthComp->OnStaminaChanged.AddDynamic(HUDWidgetRef, &UPlayerHUDWidget::OnStaminaChanged);
}

ABaseEnemy* ABasePlayerController::FindClosestEnemy() const
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn) return nullptr;

	ABaseEnemy* ClosestEnemy = nullptr;
	float ClosestDistSq = FMath::Square(EnemyHPBarShowDistance);
	FVector MyLoc = MyPawn->GetActorLocation();

	for (TActorIterator<ABaseEnemy> It(GetWorld()); It; ++It)
	{
		ABaseEnemy* Enemy = *It;
		if (!Enemy || Enemy->IsDead()) continue;

		const float DistSq = FVector::DistSquared(MyLoc, Enemy->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			ClosestEnemy = Enemy;
		}
	}
	return ClosestEnemy;
}

void ABasePlayerController::UpdateEnemyHPBar()
{
	if (GameStartWidgetRef && GameStartWidgetRef->IsVisible())
	{
		if (EnemyHPBarWidget) EnemyHPBarWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	ABaseEnemy* ClosestEnemy = FindClosestEnemy();
	if (!ClosestEnemy)
	{
		if (EnemyHPBarWidget) EnemyHPBarWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	GetOrCreateWidget(EnemyHPBarWidget, EnemyHPBarWidgetClass);
	if (EnemyHPBarWidget)
	{
		if (EnemyHPBarWidget->GetTargetEnemy() != ClosestEnemy)
		{
			EnemyHPBarWidget->SetTargetEnemy(ClosestEnemy);
		}
		if (!EnemyHPBarWidget->IsVisible())
		{
			EnemyHPBarWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}