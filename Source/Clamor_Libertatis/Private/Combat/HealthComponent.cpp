// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/HealthComponent.h"

void FResourceStat::InitializeToMax()
{
	MaxValue = FMath::Max(0.0f, MaxValue);
	CurrentValue = MaxValue;
}

void FResourceStat::SetMaxValue(float NewMaxValue, bool bRefill)
{
	MaxValue = FMath::Max(0.0f, NewMaxValue);
	CurrentValue = bRefill ? MaxValue : FMath::Clamp(CurrentValue, 0.0f, MaxValue);
}

bool FResourceStat::Consume(float Amount, bool bAllowPartialConsume)
{
	if (Amount <= 0.0f)
	{
		return true;
	}

	if (bAllowPartialConsume)
	{
		if (CurrentValue <= 0.0f)
		{
			return false;
		}

		CurrentValue = FMath::Clamp(CurrentValue - Amount, 0.0f, MaxValue);
		return true;
	}

	if (CurrentValue < Amount)
	{
		return false;
	}

	CurrentValue = FMath::Clamp(CurrentValue - Amount, 0.0f, MaxValue);
	return true;
}

bool FResourceStat::Recover(float Amount)
{
	if (Amount <= 0.0f)
	{
		return false;
	}

	const float PreviousValue = CurrentValue;
	CurrentValue = FMath::Clamp(CurrentValue + Amount, 0.0f, MaxValue);
	return !FMath::IsNearlyEqual(CurrentValue, PreviousValue);
}

float FResourceStat::GetRatio() const
{
	return MaxValue > 0.0f ? CurrentValue / MaxValue : 0.0f;
}

bool FResourceStat::IsEmpty() const
{
	return CurrentValue <= 0.0f;
}

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Health.MaxValue = 5000.0f;
	Health.InitializeToMax();
	Stamina.InitializeToMax();
	Mana.InitializeToMax();
	bDead = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health.InitializeToMax();
	Stamina.InitializeToMax();
	Mana.InitializeToMax();
	bDead = Health.IsEmpty();

	OnHealthChanged.Broadcast(Health.CurrentValue, Health.MaxValue);
	OnStaminaChanged.Broadcast(Stamina.CurrentValue, Stamina.MaxValue);
	OnManaChanged.Broadcast(Mana.CurrentValue, Mana.MaxValue);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsStaminaRegenLocked == false)
	{
		RegenStamina(DeltaTime);
	}
}

void UHealthComponent::SetMaxHealth(float newHealth)
{
	Health.SetMaxValue(newHealth, true);
	bDead = false;
	OnHealthChanged.Broadcast(Health.CurrentValue, Health.MaxValue);
}

float UHealthComponent::GetMaxHealth() const
{
	return Health.MaxValue;
}
float UHealthComponent::GetCurrentHealth() const
{
	return Health.CurrentValue;
}
void UHealthComponent::TakeDamageValue(float Damage)
{
	if (bDead || Damage <= 0.0f)
	{
		return;
	}

	Health.Consume(Damage, true);

	OnHealthChanged.Broadcast(Health.CurrentValue, Health.MaxValue);
	if (Health.IsEmpty())
	{
		bDead = true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Heal(float Amount)
{
	if (Amount <= 0 || bDead)
		return;
	if (Health.Recover(Amount))
	{
		OnHealthChanged.Broadcast(Health.CurrentValue, Health.MaxValue);
	}
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
	const bool bCanConsume = Stamina.Consume(Amount, true);
	if (!bCanConsume)
	{
		return false;
	}

	OnStaminaChanged.Broadcast(Stamina.CurrentValue, Stamina.MaxValue);

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

float UHealthComponent::GetMaxStamina() const
{ 
	return Stamina.MaxValue; 
}

float UHealthComponent::GetCurrentStamina() const
{
	return Stamina.CurrentValue;
}

void UHealthComponent::ManaHeal(float Amount)
{
	if (Amount <= 0 || bDead)
		return;
	if (Mana.Recover(Amount))
	{
		OnManaChanged.Broadcast(Mana.CurrentValue, Mana.MaxValue);
	}
}


bool UHealthComponent::ConsumeMana(float Amount)
{
	if (bDead)
	{
		return false;
	}

	if (Amount <= 0.0f)
	{
		return true;
	}

	const bool bCanConsume = Mana.Consume(Amount, false);
	if (bCanConsume)
	{
		OnManaChanged.Broadcast(Mana.CurrentValue, Mana.MaxValue);
	}

	return bCanConsume;
}
float UHealthComponent::GetMaxMana() const
{
	return Mana.MaxValue;
}
float UHealthComponent::GetCurrentMana() const
{
	return Mana.CurrentValue;
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
	if (Stamina.Recover(RegenerateStaminaPerSecond * DeltaTime))
	{
		OnStaminaChanged.Broadcast(Stamina.CurrentValue, Stamina.MaxValue);
	}
}
