// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;
	bDead = false;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::SetMaxHealth(float newHealth)
{
	MaxHealth = newHealth;
	CurrentHealth = MaxHealth;
	bDead = false;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::TakeDamageValue(float Damage)
{
	if (bDead || Damage <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	if (CurrentHealth <= 0.0f)
	{
		bDead = true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Heal(float Amount)
{
	if (Amount <= 0 || bDead)
		return;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

float UHealthComponent::GetHealthRatio() const
{
	if (MaxHealth <= 0.0f)
	{
		return 0.0f;
	}

	return CurrentHealth / MaxHealth;
}

bool UHealthComponent::ConsumeStamina(float Amount)
{
	//소울류 게임은 스테미너가 1이라도있으면 사용가능
	bool b = CurrentStamina > 0.f;


	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Green, FString::Printf(TEXT("Stamina:%f"), CurrentStamina));

	StartStaminaLock();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			StaminaRegenLockHandle,
			this,
			&UHealthComponent::UnlockStaminaRegen,
			StaminaRegenLockTime,
			false
		);
	}
	return b;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsStaminaRegenLocked == false)
	{
		RegenStamina(DeltaTime);
	}
}

void UHealthComponent::StartStaminaLock()
{
	bIsStaminaRegenLocked = true;
}

void UHealthComponent::UnlockStaminaRegen()
{
	bIsStaminaRegenLocked = false;
}

void UHealthComponent::RegenStamina(float DeltaTime)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + (RegenerateStaminaPerSecond * DeltaTime), 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Green, FString::Printf(TEXT("Stamina:%f"), CurrentStamina));
}

