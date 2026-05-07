#include "Enemy/BaseEnemy.h"

#include "BrainComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/AIController/Enemy_AIController.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/E_Weapon/Enemy_BaseWeapon.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Enemy_StatComp = CreateDefaultSubobject<UEnemy_StatComponent>(TEXT("StatComponent"));
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

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Enemy_StatComp)
	{
		if (Enemy_StatComp->GetEnemyStat().SetHP(ActualDamage))
		{
			// Enemy is Not Dead
		}
		else
		{
			OnDead();
		}
	}
	return ActualDamage;
}

void ABaseEnemy::AttackToPlayer()
{
	UE_LOG(LogTemp,Warning,TEXT("Enemy Attack Started"));
}

void ABaseEnemy::OnDead()
{
	UE_LOG(LogTemp,Warning,TEXT("%s Was Dead"),*GetName());
	
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && AIC->GetBrainComponent())
	{
		AIC->GetBrainComponent()->StopLogic(TEXT("Because Owner Was Dead"));
	}
}
