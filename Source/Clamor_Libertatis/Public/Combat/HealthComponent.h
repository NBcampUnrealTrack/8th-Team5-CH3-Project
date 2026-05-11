// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, CurrentStamina, float, MaxStamina);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAMOR_LIBERTATIS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	float MaxHealth = 5000.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stamina")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float RegenerateStaminaPerSecond = 45.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stamina")
	bool bIsStaminaRegenLocked = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float StaminaRegenLockTime = 1.5f;


protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float newHealth);

	UFUNCTION(BlueprintCallable)
	void TakeDamageValue(float Damage);

	UFUNCTION(BlueprintCallable)
	void Heal(float Amount);

	UFUNCTION(BlueprintCallable)
	float GetHealthRatio() const;

	UFUNCTION(BlueprintCallable)
	bool ConsumeStamina(float Amount);

	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentStamina() const;

	UFUNCTION(BlueprintPure)
	float GetMaxStamina() const;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChanged OnStaminaChanged;


private:
	void StartStaminaLock();
	void UnlockStaminaRegen();
	void RegenStamina(float DeltaTime);

	FTimerHandle StaminaRegenLockHandle;
		
};
