#include "Enemy/Boss/BossEnemy_Mage.h"
#include "Combat/BaseThrowMagic.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ABossEnemy_Mage::ABossEnemy_Mage()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ABossEnemy_Mage::BeginPlay()
{
	Super::BeginPlay();
}


void ABossEnemy_Mage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABossEnemy_Mage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABossEnemy_Mage::AttackHitCheck()
{
	const FEnemySkillInfo* SkillInfo = GetCurrentSkillInfo();
	if (SkillInfo && SkillInfo->bIsLaunch)
	{
		SpawnProjectile(*SkillInfo);
	}
	else
	{
		Super::AttackHitCheck();
	}
}
void ABossEnemy_Mage::SpawnProjectile(const FEnemySkillInfo& SkillInfo)
{
	if (!ProjectileClass) return;

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FName SocketName = GetProjectileSpawnSocket();
	FVector SpawnLocation = (SocketName != NAME_None && MeshComp->DoesSocketExist(SocketName))
		? MeshComp->GetSocketLocation(SocketName)
		: GetActorLocation();

	FVector DirectionToPlayer = (PlayerPawn->GetActorLocation() - SpawnLocation).GetSafeNormal();
	FRotator SpawnRotation = DirectionToPlayer.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	ABaseThrowMagic* Projectile = GetWorld()->SpawnActor<ABaseThrowMagic>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (!Projectile) return;

	Projectile->DamageAmount = GetCurrentAttackDamage();

	if (SkillInfo.bIsHoming)
	{
		UProjectileMovementComponent* PMC = Projectile->ProjectileMovementComponent;
		if (PMC)
		{
			PMC->bIsHomingProjectile = true;
			PMC->HomingTargetComponent = PlayerPawn->GetRootComponent();
			PMC->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
		}
	}
}

FName ABossEnemy_Mage::GetProjectileSpawnSocket() const
{
	return FName("MagicSocket");
}

