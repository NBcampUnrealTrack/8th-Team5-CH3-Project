#include "Enemy/Normal/NormalEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "GameFramework/CharacterMovementComponent.h"


ANormalEnemy::ANormalEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANormalEnemy::OnDead()
{
	Super::OnDead();
}

float ANormalEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (!bIsDead) HitReaction();
	
	return ActualDamage;
}

void ANormalEnemy::HitReaction()
{
	UE_LOG(LogTemp,Warning,TEXT("HitReaction"));
	bIsInHitReaction = true;
	
	if (AnimInst)
	{
		AnimInst->StopAllMontages(0.1f);

		if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
		{
			MoveComp->StopMovementImmediately();
			MoveComp->ClearAccumulatedForces();
			
			FRotator OriginRot = GetActorRotation();
			FRotator NewRot = FRotator(0.f,OriginRot.Yaw,0.f);
			SetActorRotation(NewRot);
		}
	}
	
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
		{
			BB->SetValueAsBool(TEXT("bCanMove"), false);
			BB->SetValueAsBool(TEXT("bCanAttack"), false);
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

void ANormalEnemy::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsInHitReaction = false;

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
		{
			BB->SetValueAsBool(TEXT("bCanMove"), true);
			UE_LOG(LogTemp,Warning,TEXT("bCanMove true"));
		}
	}
}

void ANormalEnemy::AttackHitCheck()
{
	Super::AttackHitCheck();
}


UAnimMontage* ANormalEnemy::AttackToPlayer()
{
	return Super::AttackToPlayer();
}