// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	bDead = false;
}

void UHealthComponent::SetMaxHealth(float newHealth)
{
	MaxHealth = FMath::Max(0.0f, newHealth);
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
	if (bDead)
	{
		return false;
	}

	if (Amount <= 0.0f)
	{
		return true;
	}

	//소울류 게임은 스테미너가 1이라도있으면 사용가능
	const bool bCanConsume = CurrentStamina > 0.f;
	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

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
	return bCanConsume;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}
float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}
float UHealthComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}
float UHealthComponent::GetMaxStamina() const
{ 
	return MaxStamina; 
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxHealth = FMath::Max(0.0f, MaxHealth);
	CurrentHealth = MaxHealth;
	bDead = CurrentHealth <= 0.0f;

	MaxStamina = FMath::Max(0.0f, MaxStamina);
	CurrentStamina = MaxStamina;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
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
	const float PreviousStamina = CurrentStamina;
	CurrentStamina = FMath::Clamp(CurrentStamina + (RegenerateStaminaPerSecond * DeltaTime), 0.f, MaxStamina);

	if (!FMath::IsNearlyEqual(CurrentStamina, PreviousStamina))
	{
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}
}

