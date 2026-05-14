// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, CurrentStamina, float, MaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, CurrentMana, float, MaxMana);

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FResourceStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource")
	float MaxValue = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Resource")
	float CurrentValue = 100.f;

	void InitializeToMax();
	void SetMaxValue(float NewMaxValue, bool bRefill);
	bool Consume(float Amount, bool bAllowPartialConsume);
	bool Recover(float Amount);
	float GetRatio() const;
	bool IsEmpty() const;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAMOR_LIBERTATIS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	FResourceStat Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	FResourceStat Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float RegenerateStaminaPerSecond = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stamina")
	bool bIsStaminaRegenLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float StaminaRegenLockTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mana")
	FResourceStat Mana;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
#pragma region Health
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float newHealth);
	
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	void TakeDamageValue(float Damage);

	UFUNCTION(BlueprintCallable)
	void Heal(float Amount);
#pragma endregion Health

#pragma region Stamina
	UFUNCTION(BlueprintCallable)
	bool ConsumeStamina(float Amount);
	
	UFUNCTION(BlueprintPure)
	float GetMaxStamina() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentStamina() const;
#pragma endregion Stamina
	
#pragma region Mana
	UFUNCTION(BlueprintCallable)
	void ManaHeal(float Amount);

	UFUNCTION(BlueprintCallable)
	bool ConsumeMana(float Amount);
	
	UFUNCTION(BlueprintPure)
	float GetMaxMana() const;
	
	UFUNCTION(BlueprintPure)
	float GetCurrentMana() const;
#pragma endregion Mana


	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnManaChanged OnManaChanged;


private:
	void StartStaminaLock();
	void UnlockStaminaRegen();
	void RegenStamina(float DeltaTime);

	FTimerHandle StaminaRegenLockHandle;
		
};
