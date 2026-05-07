#include "Enemy/Boss/BossEnemy.h"

#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/DataTable/DA_BaseEnemyAnim.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"


ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossEnemy::OnDead()
{
	Super::OnDead();
}

void ABossEnemy::AttackHitCheck()
{
	Super::AttackHitCheck();
	
	if (Enemy_CombatComp)
	{
		FVector StartPos = GetActorLocation() + (GetActorForwardVector() * Enemy_CombatComp->GetAttackDistance(CurrentMontage));
		FCollisionShape AttackCollision = Enemy_CombatComp->MakeAttackCollision(CurrentMontage);
		FQuat Rotation = GetActorRotation().Quaternion();
		
		TArray<FOverlapResult> OverlapResults;
		//TODO::Need to Custom TraceChannel
		GetWorld()->OverlapMultiByChannel(OverlapResults,StartPos, Rotation, ECC_Visibility, AttackCollision);
		DrawDebugBox(GetWorld(),StartPos,AttackCollision.GetExtent(),FColor::Red,false,2.f,0,1.f);
		
		TArray<AActor*> AlreadyHitActors;
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			if (!AlreadyHitActors.Contains(OverlapResult.GetActor()) && OverlapResult.GetActor()->ActorHasTag(TEXT("Player")))
			{
				UGameplayStatics::ApplyDamage(OverlapResult.GetActor(), Enemy_StatComp->GetEnemyStat().Attack_Damage,GetController(),this,UDamageType::StaticClass());
				UE_LOG(LogTemp,Warning,TEXT("Enemy Hit Player %s"),*OverlapResult.GetActor()->GetName());
				AlreadyHitActors.Add(OverlapResult.GetActor());
			}
		}
	}
	
}

void ABossEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
	
	//TODO:: 리팩토링 고민중
	int64 RandomNum = FMath::RandRange(1,10);
	UE_LOG(LogTemp,Warning,TEXT("RandomNum %lld"),RandomNum);
	if (RandomNum % 2 == 0)
	{
		AnimInst->Montage_Play(Enemy_CombatComp->GetAnimMontage(EAnimMontage::AM_SingleAttack));
		CurrentMontage = EAnimMontage::AM_SingleAttack;
	}
	else
	{
		AnimInst->Montage_Play(Enemy_CombatComp->GetAnimMontage(EAnimMontage::AM_3ComboAttack));
		CurrentMontage = EAnimMontage::AM_3ComboAttack;
	}
}