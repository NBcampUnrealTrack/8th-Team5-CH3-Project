// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class ACharacter;
class AWeaponBase;
class UHealthComponent;
class UAnimMontage;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAMOR_LIBERTATIS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION()
	void BasicAttack();//플레이어입력받아 공격시작/콤보이행

	UFUNCTION()
	void EnableCombo();//콤보 입력 가능
	UFUNCTION()
	void DisableCombo();//콤보 입력 불가
	UFUNCTION()
	void CheckCombo();//다음공격으로 이어가는지 체크
	UFUNCTION()
	void EnableWeaponHitbox();
	UFUNCTION()
	void DisableWeaponHitbox();
	UFUNCTION()
	void EndAttack();

	void SetCurrentWeapon(AWeaponBase* NewWeapon);
	AWeaponBase* GetCurrentWeapon() const;

	int32 GetCurrentComboIndex() const;
	float GetCurrentAttackDamage() const;
	float GetCurrentAttackStaminaCost() const;


private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UHealthComponent* HealthComponent;

	UPROPERTY()
    AWeaponBase* CurrentWeapon;

	void StartAttack();

	void JumpToComboSection(int32 InComboIndex);
	UAnimMontage* GetCurrentAttackMontage() const;
	int32 GetMaxComboCount() const;
	FName GetComboSectionName(int32 InComboIndex) const;
	
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool bIsAttacking = false;//공격 재생중
	bool bIsComboEnabled = false;//콤보입력가능한지
	bool bComboInputBuffered = false;//콤보입력했는지
	bool bIsAttackEnding = false;//종료중인지
	bool bAttackInputBufferedDuringRecovery = false;//종료 딜레이 중 입력
	int32 ComboIndex = 0;

};
