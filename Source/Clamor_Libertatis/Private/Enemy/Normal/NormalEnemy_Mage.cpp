#include "Enemy/Normal/NormalEnemy_Mage.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "GameFramework/CharacterMovementComponent.h"

ANormalEnemy_Mage::ANormalEnemy_Mage()
{
	PrimaryActorTick.bCanEverTick = true;
}

float ANormalEnemy_Mage::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsDead) HitReaction();

	return ActualDamage;
}

void ANormalEnemy_Mage::HitReaction()
{
	if (AnimInst)
	{
		AnimInst->StopAllMontages(0.1f);

		if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
		{
			MoveComp->StopMovementImmediately();
			MoveComp->ClearAccumulatedForces();

			FRotator OriginRot = GetActorRotation();
			SetActorRotation(FRotator(0.f, OriginRot.Yaw, 0.f));
		}
	}

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (AIC->GetBlackboardComponent())
		{
			AIC->GetBlackboardComponent()->SetValueAsBool(TEXT("bCanMove"), false);
		}
	}

	if (AnimInst)
	{
		if (UAnimMontage* AM_Hit = Enemy_CombatComp->DA_EnemySkill->AM_HitReaction)
		{
			FOnMontageEnded EndHitMontage;
			EndHitMontage.BindWeakLambda(this, [this](UAnimMontage* Montage, bool bInterrupted)
			{
				OnHitMontageEnded(Montage, bInterrupted);
			});
			AnimInst->Montage_Play(AM_Hit);
			AnimInst->Montage_SetEndDelegate(EndHitMontage, AM_Hit);
		}
	}
}

void ANormalEnemy_Mage::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
		{
			BB->SetValueAsBool(TEXT("bCanMove"), true);
		}
	}
}