// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/SkillComponent.h"
#include "Combat/BaseThrowMagic.h"
#include "Combat/Data/DA_SkillData.h"
#include "Combat/HealthComponent.h"
#include "Animation/AnimInstance.h"
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
		return true;
	}

	ExecuteSkill(SkillData);
	return true;
}

void USkillComponent::ExecutePendingSkill()
{
	UDA_SkillData* SkillData = PendingSkillData;
	PendingSkillData = nullptr;

	if (!SkillData)
	{
		return;
	}

	ExecuteSkill(SkillData);
}

void USkillComponent::ExecuteAttack()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	if (!OwnerCharacter)
	{
		return;
	}
	if (!PendingSkillData)
	{
		return;
	}

	FVector StartPos = OwnerCharacter->GetActorLocation();
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(PendingSkillData->SphereRadius);
	FQuat Rotation = OwnerCharacter->GetActorRotation().Quaternion();
	TArray<FHitResult> HitResults;
	TArray<AActor*> IgnoreActors;

	IgnoreActors.Add(OwnerCharacter);


	if (OwnerCharacter->GetInstigator())
	{
		IgnoreActors.Add(OwnerCharacter->GetInstigator());
	}


	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors);
	QueryParams.bTraceComplex = false;
	//TODO::Need to Custom TraceChannel
	const bool bHit = World->SweepMultiByChannel(
		HitResults,
		StartPos,
		StartPos,
		FQuat::Identity,
		ECC_Pawn,//<-Pawn 만 체크하는거임
		CollisionShape,
		QueryParams
	);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	DrawDebugSphere(GetWorld(), StartPos, PendingSkillData->SphereRadius, 32, FColor::Red, false, 2.f);
#endif

	AController* InstigatorController = OwnerCharacter->GetInstigatorController();
	TArray<AActor*> DamagedActors;
	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();

		if (!HitActor)
		{
			continue;
		}

		if (DamagedActors.Contains(HitActor))
		{
			continue;
		}
		DamagedActors.Add(HitActor);

// 		if (HitActor->ActorHasTag(TEXT("Enemy")))
// 		{
// 		}
		UGameplayStatics::ApplyDamage(
			HitActor,
			PendingSkillData->Damage,
			InstigatorController,
			OwnerCharacter,
			UDamageType::StaticClass()
		);
	}
}

void USkillComponent::PlayEffect()
{
	if (!OwnerCharacter)
	{
		return;
	}
	const FVector PlayLocation = OwnerCharacter->GetActorLocation();

	if (PendingSkillData && PendingSkillData->CastingEffects[0])
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			PendingSkillData->CastingEffects[0],
			PlayLocation,
			PlayLocation.Rotation(),
			FVector(2.f)
		);
		if (NiagaraComp)
		{
			//크기 파라미터 즉시수정.
			NiagaraComp->SetVariableFloat(TEXT("Scale_All"), 2.f);
		}
	}
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

	if (World->GetTimeSeconds() < SkillCooldownEndTime)
	{
		return false;
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
	if (HealthComponent->GetCurrentStamina() > 0 && HealthComponent->GetCurrentMana() >= SkillData->ManaCost) {
		HealthComponent->ConsumeStamina(SkillData->StaminaCost);
		HealthComponent->ConsumeMana(SkillData->ManaCost);
	}


	return HealthComponent->ConsumeStamina(SkillData->StaminaCost);
}

void USkillComponent::StartCooldown(const UDA_SkillData* SkillData)
{
	if (!SkillData)
	{
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		SkillCooldownEndTime = World->GetTimeSeconds() + FMath::Max(0.0f, SkillData->Cooldown);
		OnSkillCooldownStart.Broadcast(SkillData->Cooldown);
	}
}

void USkillComponent::ExecuteSkill(const UDA_SkillData* SkillData)
{
	if (!SkillData)
	{
		return;
	}

	switch (SkillData->SkillType)
	{
	case ESkillType::Projectile:
		SpawnProjectileSkill(SkillData);
		break;
	default:
		break;
	}
}

void USkillComponent::SpawnProjectileSkill(const UDA_SkillData* SkillData)
{
	if (!OwnerCharacter || !SkillData || !SkillData->ProjectileClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector SpawnLocation = GetSkillSpawnLocation();
	const FVector TargetLocation = GetAimTargetLocation(SkillData);
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
		SkillData->ProjectileClass,
		SpawnLocation,
		FireDirection.Rotation(),
		SpawnParams
	);

	if (ABaseThrowMagic* ThrowMagic = Cast<ABaseThrowMagic>(SpawnedActor))
	{
		ThrowMagic->DamageAmount = SkillData->Damage;
		if (SkillData->SphereRadius > 0.0f)
		{
			ThrowMagic->ExplosionRadius = SkillData->SphereRadius;
		}
	}
}

FVector USkillComponent::GetAimTargetLocation(const UDA_SkillData* SkillData) const
{
	if (!OwnerCharacter || !SkillData)
	{
		return FVector::ZeroVector;
	}

	AController* Controller = OwnerCharacter->GetController();
	if (!Controller)
	{
		return OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * SkillData->SkillRange;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const float TraceDistance = FMath::Max(0.0f, SkillData->SkillRange);
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
