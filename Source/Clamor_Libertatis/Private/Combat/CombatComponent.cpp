// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
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
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::BasicAttack()
{
	if (!OwnerCharacter || !BasicAttackAnimMontage)
		return;

	if (bIsAttacking) {
		//이미 공격중
		//입력을 받을 수 있고, 콤보가 끝나지 않았다면 퍼버 true)
		if (bIsComboEnabled and ComboIndex < MaxCombo) {
			bComboInputBuffered = true;
		}
	}
	else {
		StartAttack();
	}
}

void UCombatComponent::StartAttack()
{
	if (OwnerCharacter->GetMesh()) {
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (BasicAttackAnimMontage and AnimInstance) {
			//입력받을 세팅 초기화
			bIsAttacking = true;
			bIsComboEnabled = true;
			bComboInputBuffered = false;
			ComboIndex = 1;
			AnimInstance->Montage_Play(BasicAttackAnimMontage);

			// 				FOnMontageEnded EndDelegate;
			// 				EndDelegate.BindUObject(this, &ATestCharacter::OnAttackEnded);
			UPlayMontageCallbackProxy* Proxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(OwnerCharacter->GetMesh(), BasicAttackAnimMontage);
			if (Proxy)
			{
				Proxy->OnCompleted.AddDynamic(this, &UCombatComponent::EndAttack); // 여기에 연결된 함수들 등록
				Proxy->OnInterrupted.AddDynamic(this, &UCombatComponent::EndAttack);
				Proxy->OnNotifyBegin.AddDynamic(this, &UCombatComponent::CheckCombo);
			}
		}
	}
}

void UCombatComponent::EndAttack(FName NotifyName)
{
	ComboIndex = 0;
	bIsAttacking = false;
	bComboInputBuffered = false;
	bIsComboEnabled = false;
	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, BasicAttackAnimMontage);
}

void UCombatComponent::EnableCombo()
{
	bIsComboEnabled = true;
}

void UCombatComponent::CloseCombo()
{
	bIsComboEnabled = false;
}

void UCombatComponent::CheckCombo(FName NotifyName)
{
	if (bComboInputBuffered) {
		ComboIndex = 1;
		bComboInputBuffered = false;
	}
	else {
		EndAttack(NotifyName);
	}
}

