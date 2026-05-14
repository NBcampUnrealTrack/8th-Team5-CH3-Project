#include "Enemy/BaseEnemy.h"
#include "Components/WidgetComponent.h"
#include "UI/DamageTextWidget.h"
#include "BrainComponent.h"
#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/AIController/Enemy_AIController.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/E_Weapon/Enemy_BaseWeapon.h"

#include "Character/BasePlayerController.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Enemy_StatComp = CreateDefaultSubobject<UEnemy_StatComponent>(TEXT("StatComponent"));
	
	Enemy_CombatComp = CreateDefaultSubobject<UEnemy_CombatComponent>(TEXT("CombatComponent"));
	
	bIsDead = false;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	EquipWeapon();
	if (Enemy_StatComp)
	{
		UE_LOG(LogTemp,Warning,TEXT("Initialized Stat"));
		Enemy_StatComp->InitializeStat();

	}
	if (AEnemy_AIController* AIC = Cast<AEnemy_AIController>(GetController()))
	{
		AIC->InitializeAIPerceptionComponent();
	}
	if (GetMesh()->GetAnimInstance())
	{
		AnimInst = Cast<UBaseEnemyAnimInst>(GetMesh()->GetAnimInstance());
	}
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::EquipWeapon()
{
	if (Enemy_WeaponClass)
	{
		Enemy_WeaponInst = Cast<AEnemy_BaseWeapon>(GetWorld()->SpawnActor<AActor>(Enemy_WeaponClass,FVector::ZeroVector,FRotator::ZeroRotator));
		if (Enemy_WeaponInst)
		{
			FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		
			Enemy_WeaponInst->AttachToComponent(GetMesh(),AttachmentTransformRules,TEXT("WeaponSocket"));
			UE_LOG(LogTemp,Warning,TEXT("Enemy Weapon Initialized"));
		}
	}
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.f;
	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Enemy_StatComp)
	{
		if (Enemy_StatComp->GetEnemyStat().SetHP(ActualDamage))
		{
			// Enemy is Not Dead
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Enemy Was Dead"));
			bIsDead = true;
			OnDead();
		}
		Enemy_StatComp->OnHPChanged.Broadcast(
			Enemy_StatComp->GetEnemyStat().HP,
			Enemy_StatComp->GetEnemyStat().MaxHP);
	}
	
	if (DamageTextActorClass)
	{
		FVector SpawnLocation = GetActorLocation()
			+ GetActorForwardVector() * 50.f
			+ FVector(0.f, 0.f, 100.f);

		AActor* DamageActor = GetWorld()->SpawnActor<AActor>(
			DamageTextActorClass,
			SpawnLocation,
			FRotator::ZeroRotator
		);

		if (DamageActor)
		{
			UWidgetComponent* WidgetComp =
				DamageActor->FindComponentByClass<UWidgetComponent>();

			if (WidgetComp)
			{
				UDamageTextWidget* DamageWidget =
					Cast<UDamageTextWidget>(
						WidgetComp->GetUserWidgetObject()
					);

				if (DamageWidget)
				{
					DamageWidget->InitDamageText(DamageAmount);
				}
			}

			DamageActor->SetLifeSpan(2.f);
		}
	}
	return DamageAmount;
}

UAnimMontage* ABaseEnemy::AttackToPlayer()
{
	UE_LOG(LogTemp,Warning,TEXT("Enemy Attack Started"));

	int32 SkillCount = Enemy_CombatComp->GetSkillCount(EAttackType::Attack_Normal);
	int32 RandomNum = FMath::RandRange(0,SkillCount - 1);

	if (SkillCount > 0)
	{
		UAnimMontage* Montage = Enemy_CombatComp->GetAttackMontage(EAttackType::Attack_Normal,RandomNum);
		AnimInst->Montage_Play(Montage);
		CurrentAttackData.Key = EAttackType::Attack_Normal;
		CurrentAttackData.Value = RandomNum;
		return Montage;
	}
	return nullptr;
}

void ABaseEnemy::AttackHitCheck()
{
	UE_LOG(LogTemp,Warning,TEXT("AttackHitCheckOn"));
	
	if (Enemy_CombatComp)
	{
		FVector StartPos = GetActorLocation() + (GetActorForwardVector() * Enemy_CombatComp->GetAttackDistance(CurrentAttackData.Key,CurrentAttackData.Value));
		FCollisionShape AttackCollision = Enemy_CombatComp->MakeAttackCollision(CurrentAttackData.Key,CurrentAttackData.Value);
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
				UGameplayStatics::ApplyDamage(OverlapResult.GetActor(), GetCurrentAttackDamage(),GetController(),this,UDamageType::StaticClass());
				UE_LOG(LogTemp,Warning,TEXT("Enemy Hit Player %s"),*OverlapResult.GetActor()->GetName());
				AlreadyHitActors.Add(OverlapResult.GetActor());
			}
		}
	}
}

float ABaseEnemy::GetCurrentAttackDamage() const
{
	return Enemy_StatComp->GetEnemyStat().Attack_Damage;
}

const FEnemySkillInfo* ABaseEnemy::GetCurrentSkillInfo() const
{
	if (!Enemy_CombatComp) return nullptr;
	return &Enemy_CombatComp->GetSkillInfo(CurrentAttackData.Key, CurrentAttackData.Value);
}

void ABaseEnemy::OnDead()
{
	UE_LOG(LogTemp,Warning,TEXT("%s Was Dead"),*GetName());
	
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && AIC->GetBrainComponent())
	{
		AIC->GetBrainComponent()->StopLogic(TEXT("Because Owner Was Dead"));
	}
	
	UAnimMontage* AM_Dead = Enemy_CombatComp->DA_EnemySkill->AM_Dead;
	
	if (UAnimMontage* CurrentMontage = AnimInst->GetCurrentActiveMontage())
	{
		UE_LOG(LogTemp,Warning,TEXT("CurrentPlay Montage %s"),*CurrentMontage->GetName())
		AnimInst->Montage_Stop(0.f,CurrentMontage);
	}
	
	if (AnimInst && AM_Dead)
	{
		AnimInst->Montage_Play(AM_Dead);
		UE_LOG(LogTemp,Warning,TEXT("PlayDeadMontage"));
	}
	
	SetLifeSpan(4.f);
}

void ABaseEnemy::Destroyed()
{
	if (Enemy_WeaponInst)
	{
		Enemy_WeaponInst->Destroy();
	}
	
	Super::Destroyed();
}
