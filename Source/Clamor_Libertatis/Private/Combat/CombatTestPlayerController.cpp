// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatTestPlayerController.h"
#include "EnhancedInputSubsystems.h"

ACombatTestPlayerController::ACombatTestPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	BasicAttackAction(nullptr),
	LookAction(nullptr)
{
}

void ACombatTestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* localPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}