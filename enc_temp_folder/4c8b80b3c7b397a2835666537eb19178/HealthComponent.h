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

	
	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category="Health")
	float CurrentHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category="Health")
	bool bDead = false;
	
	UPROPERTY(EditAnywhere, Category="Stamina")
	float MaxStamina = 100.f;
	UPROPERTY(VisibleAnywhere, Category="Stamina")
	float CurrentStamina = 100.f;
	UPROPERTY(EditAnywhere, Category="Stamina")
	float RegenerateStaminaPerSecond = 40.f;
	UPROPERTY(VisibleAnywhere, Category="Stamina")
	bool bIsStaminaRegenLocked = false;

	FTimerHandle StaminaRegenLockHandle;


	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;
	FOnStaminaChanged OnStaminaChanged;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void StartStaminaLock();
	void UnlockStaminaRegen();
	void RegenStamina(float DeltaTime);


		
};
