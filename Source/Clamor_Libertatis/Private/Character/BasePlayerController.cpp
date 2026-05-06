#include "Character/BasePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ABasePlayerController::ABasePlayerController()
	: InputMappingContext(nullptr)
	, MoveAction(nullptr)
	, JumpAction(nullptr)
	, LookAction(nullptr)
	, SprintAction(nullptr)
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
	HUDWidgetRef = CreateWidget<UUserWidget>(this, HUDWidgetClass);

	if (HUDWidgetRef)
	{
		HUDWidgetRef->AddToViewport(0);
	}

}

void ABasePlayerController::SetGameState(EGameState NewState)
{
	CurrentState = NewState;

	if (DeathWidgetRef)
		DeathWidgetRef->SetVisibility(ESlateVisibility::Hidden);

	if (VictoryWidgetRef)
		VictoryWidgetRef->SetVisibility(ESlateVisibility::Hidden);

	switch (CurrentState)
	{
	case EGameState::Room:
		if (HUDWidgetRef)
			HUDWidgetRef->SetVisibility(ESlateVisibility::Visible);
		break;

	case EGameState::Combat:
		if (HUDWidgetRef)
			HUDWidgetRef->SetVisibility(ESlateVisibility::Visible);
		break;

	case EGameState::Victory:
		ShowVictoryUI();
		break;

	case EGameState::Death:
		ShowDeathUI();
		break;
	}
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
	Mode.SetWidgetToFocus(MainMenuWidgetRef->TakeWidget());
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