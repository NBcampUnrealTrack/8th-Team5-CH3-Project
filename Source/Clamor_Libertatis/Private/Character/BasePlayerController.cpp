#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Combat/HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/PlayerHUDWidget.h"

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
}

void ABasePlayerController::SetStageState(ECheckStageResult NewState)
{
	if (DeathWidgetRef)
		DeathWidgetRef->SetVisibility(ESlateVisibility::Hidden);
	if (VictoryWidgetRef)
		VictoryWidgetRef->SetVisibility(ESlateVisibility::Hidden);

	switch (NewState)
	{
	case ECheckStageResult::NotEnd:
	{
		if (!HUDWidgetRef && HUDWidgetClass)
		{
			HUDWidgetRef = CreateWidget<UPlayerHUDWidget>(
				this,
				HUDWidgetClass
			);

			if (HUDWidgetRef)
			{
				HUDWidgetRef->AddToViewport(0);

				APlayerCharacter* PlayerCharacter =
					Cast<APlayerCharacter>(GetPawn());

				if (PlayerCharacter)
				{
					UHealthComponent* HealthComp =
						PlayerCharacter->GetHealthComponent();

					if (HealthComp)
					{
						HUDWidgetRef->OnHealthChanged(
							HealthComp->CurrentHealth,
							HealthComp->MaxHealth
						);

						HUDWidgetRef->OnStaminaChanged(
							HealthComp->CurrentStamina,
							HealthComp->MaxStamina
						);

						HealthComp->OnHealthChanged.AddDynamic(
							HUDWidgetRef,
							&UPlayerHUDWidget::OnHealthChanged
						);

						HealthComp->OnStaminaChanged.AddDynamic(
							HUDWidgetRef,
							&UPlayerHUDWidget::OnStaminaChanged
						);
					}
				}
			}
		}

		if (HUDWidgetRef)
		{
			HUDWidgetRef->SetVisibility(
				ESlateVisibility::Visible
			);
		}

		break;
	}

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
	if (!GameStartWidgetRef && GameStartWidgetClass)
	{
		GameStartWidgetRef = CreateWidget<UUserWidget>(this, GameStartWidgetClass);
		if (GameStartWidgetRef) GameStartWidgetRef->AddToViewport(0);
	}
	if (GameStartWidgetRef)
		GameStartWidgetRef->SetVisibility(ESlateVisibility::Visible);
	
	bShowMouseCursor = true;
	FInputModeUIOnly Mode;
	SetInputMode(Mode);
}

void ABasePlayerController::HideGameStartUI()
{
	if (GameStartWidgetRef)
		GameStartWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
	
	bShowMouseCursor = false;
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}

void ABasePlayerController::ShowLobbyUI()
{
	if (!LobbyWidgetRef && LobbyWidgetClass)
	{
		LobbyWidgetRef = CreateWidget<UUserWidget>(this, LobbyWidgetClass);
		if (LobbyWidgetRef) LobbyWidgetRef->AddToViewport(0);
	}
	if (LobbyWidgetRef)
		LobbyWidgetRef->SetVisibility(ESlateVisibility::Visible);
}

void ABasePlayerController::HideLobbyUI()
{
	if (LobbyWidgetRef)
		LobbyWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
}

void ABasePlayerController::ShowDeathUI()
{
	if (!DeathWidgetRef && DeathWidgetClass)
	{
		DeathWidgetRef = CreateWidget<UUserWidget>(this, DeathWidgetClass);
		if (DeathWidgetRef)
		{
			DeathWidgetRef->AddToViewport(20);
		}
	}
	if (DeathWidgetRef)
	{
		DeathWidgetRef->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABasePlayerController::HideDeathUI()
{
	if (DeathWidgetRef)
	{
		DeathWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABasePlayerController::ShowVictoryUI()
{
	if (!VictoryWidgetRef && VictoryWidgetClass)
	{
		VictoryWidgetRef = CreateWidget<UUserWidget>(this, VictoryWidgetClass);
		if (VictoryWidgetRef)
		{
			VictoryWidgetRef->AddToViewport(10);
		}
	}
	if (VictoryWidgetRef)
	{
		VictoryWidgetRef->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABasePlayerController::HideVictoryUI()
{
	if (VictoryWidgetRef)
	{
		VictoryWidgetRef->SetVisibility(ESlateVisibility::Collapsed); // Hidden보다 Collapsed가 성능상 이점이 있을 수 있습니다.
	}
}

void ABasePlayerController::ShowMainMenu()
{
	if (!MainMenuWidgetRef && MainMenuWidgetClass)
	{
		MainMenuWidgetRef = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);

		if (MainMenuWidgetRef)
		{
			MainMenuWidgetRef->AddToViewport(30);
		}
	}

	if (MainMenuWidgetRef)
	{
		MainMenuWidgetRef->SetVisibility(ESlateVisibility::Visible);
	}

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	FInputModeUIOnly Mode;
	SetInputMode(Mode);

	bShowMouseCursor = true;
}

void ABasePlayerController::ContinueGame()
{
	if (MainMenuWidgetRef)
	{
		MainMenuWidgetRef->SetVisibility(ESlateVisibility::Hidden);
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	bShowMouseCursor = false;
}

void ABasePlayerController::RestartGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
	bShowMouseCursor = false;
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ABasePlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(
		this,
		this,
		EQuitPreference::Quit,
		false
	);
}