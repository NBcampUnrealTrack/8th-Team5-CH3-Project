// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/SkillComponent.h"
#include "Combat/BaseThrowMagic.h"
#include "Combat/Data/DA_SkillData.h"
#include "Combat/CombatComponent.h"
#include "Combat/HealthComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Character/TargetLockComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY(LogSkill)

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		HealthComponent = OwnerCharacter->FindComponentByClass<UHealthComponent>();
		LockOnComponent = OwnerCharacter->FindComponentByClass<UTargetLockComponent>();
	}
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkillComponent::ActiveSkill()
{
	TryActivateSkill(DefaultSkillData);
}

bool USkillComponent::TryActivateSkill(UDA_SkillData* SkillData)
{
	if (!CanActivateSkill(SkillData))
	{
		return false;
	}

	UAnimInstance* AnimInstance = nullptr;
	if (SkillData->CastMontage)
	{
		if (!OwnerCharacter || !OwnerCharacter->GetMesh())
		{
			return false;
		}

		AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (!AnimInstance)
		{
			return false;
		}
	}

	if (!CommitSkillCost(SkillData))
	{
		return false;
	}

	StartCooldown(SkillData);

	if (SkillData->CastMontage)
	{
		PendingSkillData = SkillData;
		const float PlayedLength = AnimInstance->Montage_Play(SkillData->CastMontage);
		if (PlayedLength <= 0.0f)
		{
			PendingSkillData = nullptr;
			return false;
		}
		SetLockOnRotationOverride(false);
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &USkillComponent::OnSkillMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillData->CastMontage);
		return true;
	}

	ExecuteAllSkillEvents(SkillData);
	return true;
}


void USkillComponent::ExecuteSkillEvent(FName EventID)
{
	if (EventID.IsNone())
	{
		UE_LOG(LogSkill, Warning, TEXT("ExecuteSkillEvent failed: EventID is None."));
		return;
	}

	UDA_SkillData* SkillData = PendingSkillData;
	if (!SkillData)
	{
		UE_LOG(LogSkill, Warning, TEXT("ExecuteSkillEvent failed: no pending skill for EventID %s."), *EventID.ToString());
		return;
	}

	const FSkillEventData* FoundEvent = SkillData->Events.FindByPredicate(
		[EventID](const FSkillEventData& EventData)
		{
			return EventData.EventID == EventID;
		}
	);

	if (!FoundEvent)
	{
		UE_LOG(LogSkill, Warning, TEXT("ExecuteSkillEvent failed: EventID %s was not found in %s."),
			*EventID.ToString(),
			*GetNameSafe(SkillData));
		return;
	}

	ExecuteEvent(SkillData, *FoundEvent);
}

bool USkillComponent::CanActivateSkill(const UDA_SkillData* SkillData) const
{
	if (!OwnerCharacter || !SkillData)
	{
		return false;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	if (SkillCooldownList.Contains(SkillData->SkillName)) {
		float CooldownEndTime = SkillCooldownList[SkillData->SkillName];
		if (World->GetTimeSeconds() < CooldownEndTime){
			return false;
		}
	}

	return true;
}

bool USkillComponent::CommitSkillCost(const UDA_SkillData* SkillData) const
{
	if (!SkillData)
	{
		return false;
	}

	if (!HealthComponent)
	{
		return true;
	}
	float ManaCostMultiplier = 1.f;
	if (OwnerCharacter)
	{
		UCombatComponent* CombatComp = OwnerCharacter->FindComponentByClass<UCombatComponent>();
		if (CombatComp)
		{
			ManaCostMultiplier = CombatComp->GetManaCostMultiplier();
		}
	}


	const float ModifiedManaCost = SkillData->ManaCost * ManaCostMultiplier;
	if (HealthComponent->GetCurrentMana() < ModifiedManaCost)
	{
		return false;
	}

	if (!HealthComponent->ConsumeStamina(SkillData->StaminaCost))
	{
		return false;
	}

	if (!HealthComponent->ConsumeMana(ModifiedManaCost))
	{
		return false;
	}

	return true;
}

void USkillComponent::StartCooldown(const UDA_SkillData* SkillData)
{
	if (!SkillData)
	{
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		if (SkillCooldownList.Contains(SkillData->SkillName)) {
			SkillCooldownList[SkillData->SkillName] = World->GetTimeSeconds() + FMath::Max(0.0f, SkillData->Cooldown);
			OnMultiSkillCooldownStart.Broadcast(SkillData->SkillName, SkillData->Cooldown);
		}
		else {
			SkillCooldownList.Add(SkillData->SkillName, World->GetTimeSeconds() + FMath::Max(0.0f, SkillData->Cooldown));
			OnMultiSkillCooldownStart.Broadcast(SkillData->SkillName, SkillData->Cooldown);
		}

		//다음버전에 삭제
// 		SkillCooldownEndTime = World->GetTimeSeconds() + FMath::Max(0.0f, SkillData->Cooldown);
		OnSkillCooldownStart.Broadcast(SkillData->Cooldown);
	}
}

void USkillComponent::ExecuteAllSkillEvents(const UDA_SkillData* SkillData)
{
	if (!SkillData)
	{
		return;
	}

	for (const FSkillEventData& EventData : SkillData->Events)
	{
		ExecuteEvent(SkillData, EventData);
	}
}

void USkillComponent::SetLockOnRotationOverride(bool bEnable) const
{
	if (LockOnComponent && LockOnComponent->GetCurrentTarget())
	{
		LockOnComponent->ToggleCharacterRotationLock(bEnable);
	}
}


void USkillComponent::ExecuteEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData)
{
	if (!SkillData)
	{
		return;
	}

	switch (EventData.EventType)
	{
	case ESkillEventType::Attack:
		ExecuteAttackEvent(SkillData, EventData);
		break;
	case ESkillEventType::Effect:
		PlayEffectEvent(SkillData, EventData);
		break;
	case ESkillEventType::Projectile:
		SpawnProjectileEvent(SkillData, EventData);
		break;
	case ESkillEventType::Spawn:
		SpawnActorEvent(SkillData, EventData);
		break;
	case ESkillEventType::Sound:
		PlaySoundEvent(SkillData, EventData);
		break;
	default:
		break;
	}
}

void USkillComponent::ExecuteAttackEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData)
{
	UWorld* World = GetWorld();
	if (!World || !OwnerCharacter || !SkillData)
	{
		return;
	}

	const float Radius = EventData.CommonData.Radius;
	if (Radius <= 0.0f)
	{
		return;
	}

	const FVector StartPos = GetSkillEventLocation(EventData.CommonData);
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	TArray<FHitResult> HitResults;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	if (OwnerCharacter->GetInstigator())
	{
		QueryParams.AddIgnoredActor(OwnerCharacter->GetInstigator());
	}
	QueryParams.bTraceComplex = false;

	const bool bHit = World->SweepMultiByChannel(
		HitResults,
		StartPos,
		StartPos,
		FQuat::Identity,
		EventData.AttackData.TraceChannel,
		CollisionShape,
		QueryParams
	);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	DrawDebugSphere(World, StartPos, Radius, 32, FColor::Red, false, 2.f);
#endif

	if (!bHit)
	{
		return;
	}

	const float Damage = EventData.CommonData.Damage;
	TSubclassOf<UDamageType> DamageTypeClass = EventData.CommonData.DamageTypeClass;
	if (!DamageTypeClass)
	{
		DamageTypeClass = UDamageType::StaticClass();
	}

	AController* InstigatorController = OwnerCharacter->GetInstigatorController();
	TArray<AActor*> DamagedActors;
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor || DamagedActors.Contains(HitActor))
		{
			continue;
		}

		DamagedActors.Add(HitActor);
		UGameplayStatics::ApplyDamage(
			HitActor,
			Damage,
			InstigatorController,
			OwnerCharacter,
			DamageTypeClass
		);
	}
}

void USkillComponent::PlayEffectEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData)
{
	if (!OwnerCharacter || !SkillData)
	{
		return;
	}

	UNiagaraSystem* NiagaraSystem = EventData.EffectData.NiagaraSystem;

	if (!NiagaraSystem)
	{
		return;
	}

	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	const bool bCanAttach = EventData.EffectData.bAttachToSocket
		&& OwnerMesh
		&& !EventData.CommonData.SpawnSocketName.IsNone()
		&& OwnerMesh->DoesSocketExist(EventData.CommonData.SpawnSocketName);

	UNiagaraComponent* NiagaraComp = nullptr;
	if (bCanAttach)
	{
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystem,
			OwnerMesh,
			EventData.CommonData.SpawnSocketName,
			EventData.CommonData.LocationOffset,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}
	else
	{
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraSystem,
			GetSkillEventLocation(EventData.CommonData),
			OwnerCharacter->GetActorRotation(),
			EventData.EffectData.Scale
		);
	}

	if (NiagaraComp)
	{
		NiagaraComp->SetWorldScale3D(EventData.EffectData.Scale);
		//받은 에셋 나이아가라에 스케일설정하려면 원본을 건드려야 해서 유저파라미너 수정만 하는방식으로 사용 
		NiagaraComp->SetVariableFloat(TEXT("Scale_All"), EventData.EffectData.Scale.X);
	}
}

void USkillComponent::SpawnProjectileEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData)
{
	if (!OwnerCharacter || !SkillData)
	{
		return;
	}

	TSubclassOf<AActor> ProjectileClass = EventData.ProjectileData.ProjectileClass;
	if (!ProjectileClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector SpawnLocation = GetSkillEventLocation(EventData.CommonData);
	const float Range = EventData.CommonData.Range;
	const FVector TargetLocation = GetAimTargetLocation(Range);
	const FVector FireDirection = (TargetLocation - SpawnLocation).GetSafeNormal();
	if (FireDirection.IsNearlyZero())
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.Instigator = OwnerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(
		ProjectileClass,
		SpawnLocation,
		FireDirection.Rotation(),
		SpawnParams
	);

	if (ABaseThrowMagic* ThrowMagic = Cast<ABaseThrowMagic>(SpawnedActor))
	{
		ThrowMagic->DamageAmount = EventData.CommonData.Damage;
		const float ExplosionRadius = EventData.ProjectileData.ExplosionRadius > 0.0f
			? EventData.ProjectileData.ExplosionRadius
			: EventData.CommonData.Radius;
		if (ExplosionRadius > 0.0f)
		{
			ThrowMagic->ExplosionRadius = ExplosionRadius;
		}
	}
}

void USkillComponent::SpawnActorEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData)
{
	if (!OwnerCharacter || !SkillData || !EventData.SpawnData.SpawnClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector SpawnLocation = EventData.SpawnData.bSpawnAtAimTarget
		? GetAimTargetLocation(EventData.CommonData.Range)
		: GetSkillEventLocation(EventData.CommonData);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.Instigator = OwnerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	World->SpawnActor<AActor>(
		EventData.SpawnData.SpawnClass,
		SpawnLocation,
		OwnerCharacter->GetActorRotation(),
		SpawnParams
	);
}

void USkillComponent::PlaySoundEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData)
{
	if (!OwnerCharacter || !SkillData || !EventData.SoundData.Sound)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(
		this,
		EventData.SoundData.Sound,
		GetSkillEventLocation(EventData.CommonData),
		EventData.SoundData.VolumeMultiplier,
		EventData.SoundData.PitchMultiplier
	);
}



FVector USkillComponent::GetAimTargetLocation(float Range) const
{
	if (LockOnComponent && LockOnComponent->GetCurrentTarget()) {
		


		return LockOnComponent->GetCurrentTarget()->GetActorLocation();
	}


	if (!OwnerCharacter)
	{
		return FVector::ZeroVector;
	}

	AController* Controller = OwnerCharacter->GetController();
	if (!Controller)
	{
		return OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * Range;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const float TraceDistance = FMath::Max(0.0f, Range);
	const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * TraceDistance;

	UWorld* World = GetWorld();
	if (!World)
	{
		return TraceEnd;
	}

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.bTraceComplex = true;

	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		ViewLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	return bHit ? HitResult.ImpactPoint : TraceEnd;
}

FVector USkillComponent::GetSkillSpawnLocation() const
{
	if (!OwnerCharacter)
	{
		return FVector::ZeroVector;
	}

	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	if (OwnerMesh && OwnerMesh->DoesSocketExist(SkillSpawnSocketName))
	{
		return OwnerMesh->GetSocketLocation(SkillSpawnSocketName);
	}

	return OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 50.0f;
}

FVector USkillComponent::GetSkillEventLocation(const FSkillEventCommonData& CommonData) const
{
	if (!OwnerCharacter)
	{
		return FVector::ZeroVector;
	}

	FVector BaseLocation = FVector::ZeroVector;
	USkeletalMeshComponent* OwnerMesh = OwnerCharacter->GetMesh();
	
	if (!CommonData.SpawnSocketName.IsNone() && OwnerMesh && OwnerMesh->DoesSocketExist(CommonData.SpawnSocketName))
	{
		BaseLocation = OwnerMesh->GetSocketLocation(CommonData.SpawnSocketName);
	}
	else
	{
		BaseLocation = GetSkillSpawnLocation();
	}

	return BaseLocation + OwnerCharacter->GetActorRotation().RotateVector(CommonData.LocationOffset);
}

void USkillComponent::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (PendingSkillData)
	{
		PendingSkillData = nullptr;
	}
	if (OwnerCharacter) {
		if (UCombatComponent* CombatComponent = OwnerCharacter->FindComponentByClass<UCombatComponent>())
		{
			CombatComponent->SetSuperarmor(false);
		}
	}

	SetLockOnRotationOverride(true);
}
