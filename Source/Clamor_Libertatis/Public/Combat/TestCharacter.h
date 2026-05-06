// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestCharacter.generated.h"

struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
class UCombatComponent;
class UHealthComponent;
class AWeaponBase;

UCLASS()
class CLAMOR_LIBERTATIS_API ATestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATestCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCombatComponent> CombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;
	
	UPROPERTY(EditAnywhere, Category = "StartWeapon")
    TSubclassOf<AWeaponBase> WeaponClass;
	UPROPERTY()
	AWeaponBase* EquippedWeapon;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

	//이거 Controller 가 가지고있어야할듯.
	UFUNCTION()
	void BasicAttack(const FInputActionValue& value);
private:
	bool bAttacking = false;

};

