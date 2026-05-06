#include "Character/BasePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

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

	UE_LOG(LogTemp, Warning, TEXT("TEST: Death UI 실행"));

	SetGameState(EGameState::Death);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UE_LOG(LogTemp, Warning, TEXT("TEST: Victory UI 실행"));
			SetGameState(EGameState::Victory);
		}, 3.0f, false);
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

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void ABasePlayerController::HideMainMenu()
{
	if (MainMenuWidgetRef)
	{
		MainMenuWidgetRef->SetVisibility(ESlateVisibility::Hidden);
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}