#include "Enemy/Boss/BossEnemy_Mage.h"
#include "Combat/BaseThrowMagic.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy/BaseEnemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ABossEnemy_Mage::ABossEnemy_Mage()
{
	PrimaryActorTick.bCanEverTick = true;

	SummonSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SummonSpawnBox"));
	SummonSpawnBox->SetupAttachment(GetRootComponent());
	SummonSpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SummonSpawnBox->SetBoxExtent(FVector(300.f, 300.f, 50.f));
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
	if (SkillInfo && SkillInfo->bIsLaserAttack)
	{
		FireLaser(*SkillInfo);
	}
	else if (SkillInfo && SkillInfo->bIsLaunch)
	{
		SpawnProjectile(*SkillInfo);
	}
	else
	{
		Super::AttackHitCheck();
	}
}

void ABossEnemy_Mage::FireLaser(const FEnemySkillInfo& SkillInfo)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	FName SocketName = GetProjectileSpawnSocket();
	// FVector Start = (SocketName != NAME_None && MeshComp->DoesSocketExist(SocketName))
	// 	? MeshComp->GetSocketLocation(SocketName)
	// 	: GetActorLocation();
	FVector Start = GetActorLocation();

	FVector AimDirection = GetActorForwardVector();

	FVector End = Start + AimDirection * SkillInfo.LaserRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	FCollisionShape LaserShape = FCollisionShape::MakeSphere(SkillInfo.LaserRadius);
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, ObjectQueryParams, LaserShape, Params);

	FVector BeamEnd = End;

	if (bHit && HitResult.GetActor() && HitResult.GetActor()->ActorHasTag(TEXT("Player")))
	{
		UE_LOG(LogTemp,Warning,TEXT("LaserAttack Hit On"));
		UGameplayStatics::ApplyDamage(HitResult.GetActor(), GetCurrentAttackDamage(), GetController(), this, UDamageType::StaticClass());
	}

	// if (SkillInfo.LaserVFX)
	// {
	// 	UNiagaraComponent* BeamComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	// 		this, SkillInfo.LaserVFX, Start, GetActorRotation());
	// 	if (BeamComp)
	// 	{
	// 		BeamComp->SetNiagaraVariableVec3(TEXT("BeamEnd"), BeamEnd);
	// 	}
	// }

	DrawDebugCylinder(GetWorld(), Start, BeamEnd, SkillInfo.LaserRadius, 12, FColor::Cyan, false, 2.f);
}

void ABossEnemy_Mage::SpawnProjectile(const FEnemySkillInfo& SkillInfo)
{
	if (!SkillInfo.ProjectileClass) return;

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FName SocketName = GetProjectileSpawnSocket();
	FVector SpawnLocation = (SocketName != NAME_None && MeshComp->DoesSocketExist(SocketName))
		? MeshComp->GetSocketLocation(SocketName)
		: GetActorLocation();

	FVector BaseDirection = (PlayerPawn->GetActorLocation() - SpawnLocation).GetSafeNormal();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	int32 Count = FMath::Max(1, SkillInfo.ProjectileCount);
	TArray<ABaseThrowMagic*> Spawned;

	for (int32 i = 0; i < Count; i++)
	{
		float Angle = (Count == 1) ? 0.f
			: -SkillInfo.SpreadAngle * 0.5f + i * (SkillInfo.SpreadAngle / (Count - 1));

		FVector SpreadDirection = BaseDirection.RotateAngleAxis(Angle, FVector::UpVector);

		ABaseThrowMagic* Projectile = GetWorld()->SpawnActor<ABaseThrowMagic>(
			SkillInfo.ProjectileClass,
			SpawnLocation,
			SpreadDirection.Rotation(),
			SpawnParams
		);
		if (!Projectile) continue;

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

		Spawned.Add(Projectile);
	}

	for (ABaseThrowMagic* A : Spawned)
		for (ABaseThrowMagic* B : Spawned)
			if (A != B) A->SphereComponent->IgnoreActorWhenMoving(B, true);
}

FName ABossEnemy_Mage::GetProjectileSpawnSocket() const
{
	return FName("MagicSocket");
}

void ABossEnemy_Mage::OnDead()
{
	for (ABaseEnemy* Minion : SummonMinionsArray)
	{
		if (Minion)
		{
			Minion->Destroy();
		}
	}

	Super::OnDead();
}

UAnimMontage* ABossEnemy_Mage::DodgeBackward()
{
	LaunchCharacter(-GetActorForwardVector() * DodgeSpeed, true, false);
	return nullptr;
}

void ABossEnemy_Mage::SpawnChargeProjectile(FName SocketName)
{
	const FEnemySkillInfo* SkillInfo = GetCurrentSkillInfo();
	if (!SkillInfo || !SkillInfo->ProjectileClass) return;

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	FVector SpawnLocation = (SocketName != NAME_None && MeshComp->DoesSocketExist(SocketName))
		? MeshComp->GetSocketLocation(SocketName)
		: GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	int32 Count = FMath::Max(1, SkillInfo->ProjectileCount);
	PendingProjectiles.Empty();

	for (int32 i = 0; i < Count; i++)
	{
		ABaseThrowMagic* Projectile = GetWorld()->SpawnActor<ABaseThrowMagic>(
			SkillInfo->ProjectileClass,
			SpawnLocation,
			FRotator::ZeroRotator,
			SpawnParams
		);
		if (!Projectile) continue;

		Projectile->PrepareForLaunch();
		Projectile->DamageAmount = GetCurrentAttackDamage();

		if (SocketName != NAME_None && MeshComp->DoesSocketExist(SocketName))
		{
			Projectile->AttachToComponent(MeshComp,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				SocketName);
		}

		PendingProjectiles.Add(Projectile);
	}
}

void ABossEnemy_Mage::SummonMinions()
{
	if (!SummonSpawnBox || SummonMinionClasses.IsEmpty()) return;

	FVector ActorLocation = GetActorLocation();
	FVector BoxExtent = SummonSpawnBox->GetScaledBoxExtent();
	float SpawnZ = ActorLocation.Z;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (TSubclassOf<ABaseEnemy> MinionClass : SummonMinionClasses)
	{
		if (!MinionClass) continue;

		// 액터 로컬 공간 기준으로 정면(X+) 방향에서만 랜덤 오프셋 생성
		float ForwardOffset = FMath::RandRange(50.f, (float)BoxExtent.X);
		float RightOffset   = FMath::RandRange(-(float)BoxExtent.Y, (float)BoxExtent.Y);

		FVector RandomPoint = ActorLocation
			+ GetActorForwardVector() * ForwardOffset
			+ GetActorRightVector()   * RightOffset;
		RandomPoint.Z = SpawnZ;

		ABaseEnemy* SummonedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(MinionClass, RandomPoint, FRotator::ZeroRotator, SpawnParams);
		SummonMinionsArray.Add(SummonedEnemy);
	}
}

void ABossEnemy_Mage::LaunchChargeProjectile()
{
	if (PendingProjectiles.IsEmpty()) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	const FEnemySkillInfo* SkillInfo = GetCurrentSkillInfo();
	int32 Count = PendingProjectiles.Num();

	for (int32 i = 0; i < Count; i++)
		for (int32 j = 0; j < Count; j++)
			if (i != j && PendingProjectiles[i] && PendingProjectiles[j])
				PendingProjectiles[i]->SphereComponent->IgnoreActorWhenMoving(PendingProjectiles[j], true);

	for (int32 i = 0; i < Count; i++)
	{
		ABaseThrowMagic* Projectile = PendingProjectiles[i];
		if (!Projectile) continue;

		Projectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		FVector BaseDirection = (PlayerPawn->GetActorLocation() - Projectile->GetActorLocation()).GetSafeNormal();
		float SpreadAngle = SkillInfo ? SkillInfo->SpreadAngle : 0.f;
		float Angle = (Count == 1) ? 0.f
			: -SpreadAngle * 0.5f + i * (SpreadAngle / (Count - 1));

		FVector LaunchDirection = BaseDirection.RotateAngleAxis(Angle, FVector::UpVector);
		Projectile->Launch(LaunchDirection);

		if (SkillInfo && SkillInfo->bIsHoming)
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

	PendingProjectiles.Empty();
}

