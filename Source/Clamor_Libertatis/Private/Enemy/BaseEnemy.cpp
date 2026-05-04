#include "Enemy/BaseEnemy.h"

#include "Enemy/ActorComponent/Enemy_StatComponent.h"
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
	InitializedStat();
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

void ABaseEnemy::InitializedStat()
{
	if (Enemy_StatComp)
	{
		Enemy_StatComp->InitializeEnemyStat();
		UE_LOG(LogTemp,Warning,TEXT("Enemy HP: %f, Att : %f"),Enemy_StatComp->GetEnemyHP(), Enemy_StatComp->GetEnemyAttack_Damage());
	}
}
