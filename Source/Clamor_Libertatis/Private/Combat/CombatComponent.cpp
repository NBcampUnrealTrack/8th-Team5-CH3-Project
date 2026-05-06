// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "Combat/Weapon/WeaponBase.h"
#include "Combat/HealthComponent.h"
#include "GameFramework/Character.h"
#include "PlayMontageCallbackProxy.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter)
	{
		HealthComponent = OwnerCharacter->FindComponentByClass<UHealthComponent>();
	}
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::BasicAttack()
{
	if (!OwnerCharacter || !CurrentWeapon)
		return;

	if (bIsAttacking) 
	{
		if (bIsComboEnabled and ComboIndex < GetMaxComboCount()) 
		{
			bComboInputBuffered = true;
			UE_LOG(LogTemp, Warning, TEXT("Combo Input Buffered"));
			return;
		}
		if (bIsAttackEnding)
		{
			bAttackInputBufferedDuringRecovery = true;

			UE_LOG(LogTemp, Warning, TEXT("Recovery Attack Input Buffered"));
			return;
		}
		return;
	}
	StartAttack();
}

void UCombatComponent::StartAttack()
{
	if (!OwnerCharacter)
		return;

	UAnimMontage* AttackMontage = GetCurrentAttackMontage();
	if (!AttackMontage)
		return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
		return;

	//입력받을 세팅 초기화
	bIsAttacking = true;
	bIsComboEnabled = false;//노티받고 진행
	bComboInputBuffered = false;
	bIsAttackEnding = false;
	bAttackInputBufferedDuringRecovery = false;

	ComboIndex = 1;
	if (HealthComponent)
	{
		const FWeaponAttackData* AttackData = CurrentWeapon->GetAttackData(ComboIndex);

		if (AttackData)
		{
			//스테미나 없으면 
			if (!HealthComponent->ConsumeStamina(AttackData->StaminaCost))
			{
				EndAttack();
				return;
			}
			AnimInstance->Montage_Play(AttackMontage);
			//애님 종료시 안전장치
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UCombatComponent::OnAttackMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

			JumpToComboSection(ComboIndex);
		}
	}

}

void UCombatComponent::EndAttack()
{
	DisableWeaponHitbox();
	ComboIndex = 0;
	bIsAttacking = false;
	bComboInputBuffered = false;
	bIsComboEnabled = false;
	bIsAttackEnding = false;
	bAttackInputBufferedDuringRecovery = false;

	UE_LOG(LogTemp, Warning, TEXT("EndAttack-process"));
	//몽타주에서 섹션 연결을 끊어야함
	//OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, BasicAttackAnimMontage);
}
void UCombatComponent::CheckCombo()
{
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("CheckCombo / IsAttacking=%d, ComboEnabled=%d, ComboBuffered=%d, ComboIndex=%d, MaxCombo=%d"),
		bIsAttacking,
		bIsComboEnabled,
		bComboInputBuffered,
		ComboIndex,
		GetMaxComboCount()
	);

	if (bIsAttacking == false)
		return;
	bIsComboEnabled = false;

	if (bComboInputBuffered and ComboIndex < GetMaxComboCount()) {

		if (HealthComponent)
		{
			const FWeaponAttackData* AttackData = CurrentWeapon->GetAttackData(ComboIndex);

			if (AttackData)
			{
				if (!HealthComponent->ConsumeStamina(AttackData->StaminaCost))
				{
					EndAttack();
					return;
				}
				ComboIndex++;
				bComboInputBuffered = false;
				bIsAttackEnding = false;
				JumpToComboSection(ComboIndex);
			}
		}
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("CheckCombo-EndAttack"));
	bIsAttackEnding = true;
	//EndAttack();
}
void UCombatComponent::SetCurrentWeapon(AWeaponBase* NewWeapon)
{
	CurrentWeapon = NewWeapon;
}

AWeaponBase* UCombatComponent::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

int32 UCombatComponent::GetCurrentComboIndex() const
{
	return ComboIndex;
}

float UCombatComponent::GetCurrentAttackDamage() const
{
	if (!CurrentWeapon)
		return 0.f;

	return CurrentWeapon->GetAttackDamage(ComboIndex);
}

float UCombatComponent::GetCurrentAttackStaminaCost() const
{
	if (!CurrentWeapon)
		return 0.f;

	return CurrentWeapon->GetAttackStaminaCost(ComboIndex);
}

void UCombatComponent::EnableCombo()
{
	if (!bIsAttacking)
		return;

	bIsComboEnabled = true;
}

void UCombatComponent::DisableCombo()
{
	if (!bIsAttacking)
		return;

	bIsComboEnabled = false;
}



void UCombatComponent::EnableWeaponHitbox()
{
	//공격 히트박스 On, 공격 기록 초기화
	if (!CurrentWeapon)
		return;

	CurrentWeapon->EnableHitbox();
}

void UCombatComponent::DisableWeaponHitbox()
{
	//공격 히트박스 OFF
	if (!CurrentWeapon)
		return;

	CurrentWeapon->DisableHitbox();
}

void UCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != GetCurrentAttackMontage())
		return;
	//강제취소된경우
	if (bInterrupted) {
		EndAttack();
		return;
	}

	if (bAttackInputBufferedDuringRecovery) {
		bAttackInputBufferedDuringRecovery = false;

		EndAttack();
		StartAttack();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("MontageEnded-EndAttack"));
	EndAttack();
}

void UCombatComponent::JumpToComboSection(int32 InComboIndex)
{
	UAnimMontage* AttackMontage = GetCurrentAttackMontage();

	if (!OwnerCharacter || !AttackMontage)
		return;

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	if (!AnimInstance)
		return;

	const FName SectionName = GetComboSectionName(InComboIndex);

	if (SectionName == NAME_None)
		return;

	AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
}

UAnimMontage* UCombatComponent::GetCurrentAttackMontage() const
{
	if (!CurrentWeapon)
	{
		return nullptr;
	}

	return CurrentWeapon->GetAttackMontage();
}

int32 UCombatComponent::GetMaxComboCount() const 
{
	if (!CurrentWeapon)
	{
		return 0;
	}

	return CurrentWeapon->GetMaxComboCount();
}

FName UCombatComponent::GetComboSectionName(int32 InComboIndex) const
{
// 	FString BaseName = TEXT("Attack_");
// 
// 	return FName(*FString::Printf(TEXT("%s%d"), *BaseName, InComboIndex));
	if (!CurrentWeapon)
	{
		return NAME_None;
	}

	return CurrentWeapon->GetAttackSectionName(InComboIndex);
}

