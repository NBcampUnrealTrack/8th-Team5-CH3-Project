// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/WeaponBase.h"
#include "Combat/Weapon/WeaponAttackData.h"
#include "Combat/CombatComponent.h"
#include "Combat/CombatCameraShake.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Hitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hitbox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	Hitbox->SetGenerateOverlapEvents(false);
	Hitbox->SetupAttachment(StaticMeshComponent);
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnHitboxBeginOverlap);

	InitializeDefaultSocketSlots();

	HitCameraShakeClass = UCombatCameraShake::StaticClass();
}

UAnimMontage* AWeaponBase::GetAttackMontage() const
{
	return AttackComboMontage;
}

int32 AWeaponBase::GetMaxComboCount() const
{
	return AttackDataList.Num();
}

FName AWeaponBase::GetAttackSectionName(int32 ComboIndex) const
{
	const int32 ArrayIndex = ComboIndex - 1;

	if (!AttackDataList.IsValidIndex(ArrayIndex))
	{
		return NAME_None;
	}

	return AttackDataList[ArrayIndex].SectionName;
}

const FWeaponAttackData* AWeaponBase::GetAttackData(int32 ComboIndex) const
{
	const int32 ArrayIndex = ComboIndex - 1;

	if (!AttackDataList.IsValidIndex(ArrayIndex))
	{
		return nullptr;
	}

	return &AttackDataList[ArrayIndex];
}

float AWeaponBase::GetAttackDamage(int32 ComboIndex) const
{
	const FWeaponAttackData* AttackData = GetAttackData(ComboIndex);

	if (!AttackData)
	{
		return 0.0f;
	}

	float ModifiedDamage = AttackData->Damage;
	for (const UWeaponSocketItemData* SocketItem : GetEquippedSocketItems())
	{
		ModifiedDamage = SocketItem->GetModifiedDamage(ModifiedDamage);
	}

	return ModifiedDamage;
}

float AWeaponBase::GetAttackStaminaCost(int32 ComboIndex) const
{
	const FWeaponAttackData* AttackData = GetAttackData(ComboIndex);

	if (!AttackData)
	{
		return 0.0f;
	}

	float ModifiedStaminaCost = AttackData->StaminaCost;
	for (const UWeaponSocketItemData* SocketItem : GetEquippedSocketItems())
	{
		ModifiedStaminaCost = SocketItem->GetModifiedStaminaCost(ModifiedStaminaCost);
	}

	return ModifiedStaminaCost;
}

float AWeaponBase::GetManaCostMultiplier() const
{
	float ManaCostMultiplier = 1.0f;
	for (const UWeaponSocketItemData* SocketItem : GetEquippedSocketItems())
	{
		ManaCostMultiplier = SocketItem->GetModifiedManaCost(ManaCostMultiplier);
	}

	return FMath::Max(0.01f, ManaCostMultiplier);
}

float AWeaponBase::GetAttackSpeedMultiplier() const
{
	float AttackSpeedMultiplier = 1.0f;
	for (const UWeaponSocketItemData* SocketItem : GetEquippedSocketItems())
	{
		AttackSpeedMultiplier = SocketItem->GetModifiedAttackSpeed(AttackSpeedMultiplier);
	}

	return FMath::Max(0.01f, AttackSpeedMultiplier);
}

void AWeaponBase::EnableHitbox()
{
	if (!Hitbox)
	{
		return;
	}
	HitActors.Empty();

	Hitbox->SetGenerateOverlapEvents(true);
	Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void AWeaponBase::DisableHitbox()
{
	if (!Hitbox)
	{
		return;
	}

	Hitbox->SetGenerateOverlapEvents(false);
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeaponBase::AttachToCharacterHand(ACharacter* TargetCharacter)
{
	if (!TargetCharacter)
	{
		return;
	}

	USkeletalMeshComponent* CharacterMesh = TargetCharacter->GetMesh();

	if (!CharacterMesh)
	{
		return;
	}

	static const FName WeaponSocketName = TEXT("Hand_R_Weapon");
	const FName SocketName = CharacterMesh->DoesSocketExist(WeaponSocketName) ? WeaponSocketName : NAME_None;
	AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

	SetOwner(TargetCharacter);
}

void AWeaponBase::SetWeaponTrailNiagara(UNiagaraSystem* NewTrailNiagara)
{
	TrailNiagara = NewTrailNiagara;
}

UNiagaraSystem* AWeaponBase::GetWeaponTrailNiagara() const
{
	for (const FWeaponSocketSlot& Slot : SocketSlots)
	{
		if (Slot.EquippedItem && Slot.EquippedItem->TrailNiagaraOverride)
		{
			return Slot.EquippedItem->TrailNiagaraOverride;
		}
	}

	return TrailNiagara;
}

UNiagaraSystem* AWeaponBase::GetWeaponHitNiagara() const
{

	for (const FWeaponSocketSlot& Slot : SocketSlots)
	{
		if (Slot.EquippedItem && Slot.EquippedItem->HitNiagaraOverride)
		{
			return Slot.EquippedItem->HitNiagaraOverride;
		}
	}
	return HitNiagara;
}

bool AWeaponBase::EquipSocketItem(UWeaponSocketItemData* SocketItem, EWeaponSocketType SocketTag)
{
	if (!CanEquipSocketItem(SocketItem, SocketTag))
	{
		return false;
	}

	FWeaponSocketSlot* Slot = FindSocketSlot(SocketTag);
	if (!Slot)
	{
		return false;
	}

	Slot->EquippedItem = SocketItem;
	return true;
}

UWeaponSocketItemData* AWeaponBase::UnequipSocketItem(EWeaponSocketType SocketTag)
{
	FWeaponSocketSlot* Slot = FindSocketSlot(SocketTag);
	if (!Slot)
	{
		return nullptr;
	}

	UWeaponSocketItemData* RemovedItem = Slot->EquippedItem;
	Slot->EquippedItem = nullptr;
	return RemovedItem;
}

UWeaponSocketItemData* AWeaponBase::GetEquippedSocketItem(EWeaponSocketType SocketTag) const
{
	const FWeaponSocketSlot* Slot = FindSocketSlot(SocketTag);
	return Slot ? Slot->EquippedItem : nullptr;
}

void AWeaponBase::InitializeDefaultSocketSlots()
{
	if (SocketSlots.Num() > 0)
	{
		return;
	}

	FWeaponSocketSlot BladeSlot;
	BladeSlot.SocketTag = EWeaponSocketType::Blade;
	SocketSlots.Add(BladeSlot);

	FWeaponSocketSlot GripSlot;
	GripSlot.SocketTag = EWeaponSocketType::Grip;
	SocketSlots.Add(GripSlot);
}

FWeaponSocketSlot* AWeaponBase::FindSocketSlot(EWeaponSocketType SocketTag)
{
	for (FWeaponSocketSlot& Slot : SocketSlots)
	{
		if (Slot.SocketTag == SocketTag)
		{
			return &Slot;
		}
	}

	return nullptr;
}

const FWeaponSocketSlot* AWeaponBase::FindSocketSlot(EWeaponSocketType SocketTag) const
{
	for (const FWeaponSocketSlot& Slot : SocketSlots)
	{
		if (Slot.SocketTag == SocketTag)
		{
			return &Slot;
		}
	}

	return nullptr;
}

bool AWeaponBase::CanEquipSocketItem(const UWeaponSocketItemData* SocketItem, EWeaponSocketType SocketTag) const
{
	if (!SocketItem || SocketTag == EWeaponSocketType::None)
	{
		return false;
	}

	const FWeaponSocketSlot* Slot = FindSocketSlot(SocketTag);
	if (!Slot)
	{
		return false;
	}

	return SocketItem->IsCompatibleWithSocket(SocketTag)
		&& SocketItem->IsCompatibleWithWeaponTags(WeaponTags);
}

TArray<const UWeaponSocketItemData*> AWeaponBase::GetEquippedSocketItems() const
{
	TArray<const UWeaponSocketItemData*> EquippedItems;
	for (const FWeaponSocketSlot& Slot : SocketSlots)
	{
		if (Slot.EquippedItem)
		{
			EquippedItems.Add(Slot.EquippedItem);
		}
	}

	return EquippedItems;
}

void AWeaponBase::PlayHitFX(const FHitResult& SweepResult, const AActor* HitActor) const
{
	FVector PlayLocation = GetActorLocation();
	if (SweepResult.bBlockingHit || !SweepResult.ImpactPoint.IsNearlyZero())
	{
		PlayLocation = SweepResult.ImpactPoint;
	}
	else if (HitActor)
	{
		PlayLocation = HitActor->GetActorLocation();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			PlayLocation,
			HitSoundVolume,
			HitSoundPitch
		);
	}

	UNiagaraSystem* HitNiagaraSystem = GetWeaponHitNiagara();
	UWorld* World = GetWorld();
	if (!HitNiagaraSystem || !World)
	{
		return;
	}

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		HitNiagaraSystem,
		PlayLocation,
		FRotator::ZeroRotator,
		FVector(1.f),
		true,
		true
	);

	if (NiagaraComp)
	{
		NiagaraComp->SetAutoDestroy(true);

		if (HitFXLifeTime > 0.0f)
		{
			FTimerHandle HitFXDestroyTimerHandle;
			World->GetTimerManager().SetTimer(
				HitFXDestroyTimerHandle,
				FTimerDelegate::CreateWeakLambda(NiagaraComp, [NiagaraComp]()
				{
					if (IsValid(NiagaraComp))
					{
						NiagaraComp->Deactivate();
						NiagaraComp->DestroyComponent();
					}
				}),
				HitFXLifeTime,
				false
			);
		}
	}
}

void AWeaponBase::PlayCameraShake(float Damage)
{
	if (!HitCameraShakeClass) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	APlayerController* PlayerController =
		OwnerPawn ? Cast<APlayerController>(OwnerPawn->GetController()) : nullptr;

	if (PlayerController && PlayerController->PlayerCameraManager)
	{
		PlayerController->PlayerCameraManager->StartCameraShake(
			HitCameraShakeClass,
			HitCameraShakeScaleOffset / Damage
		);
	}
}

void AWeaponBase::OnHitboxBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	AActor* OwnerActor = GetOwner();

	if (!OwnerActor || !OtherActor || OtherActor == OwnerActor || OtherActor == this)
	{
		return;
	}

	if (HitActors.Contains(OtherActor))
	{
		return;
	}



	UCombatComponent* OwnerCombatComponent = OwnerActor->FindComponentByClass<UCombatComponent>();

	if (!OwnerCombatComponent)
	{
		return;
	}

	const float Damage = OwnerCombatComponent->GetCurrentAttackDamage();
	if (Damage <= 0.0f)
	{
		return;
	}

	HitActors.Add(OtherActor);

	APawn* OwnerPawn = Cast<APawn>(OwnerActor);
	AController* InstigatorController = OwnerPawn ? OwnerPawn->GetController() : nullptr;

	const float AppliedDamage = UGameplayStatics::ApplyDamage(
		OtherActor,
		Damage,
		InstigatorController,
		OwnerActor,
		UDamageType::StaticClass()
	);

	if (AppliedDamage > 0.0f)
	{
		PlayHitFX(SweepResult, OtherActor);
		PlayCameraShake(AppliedDamage);
	}
}
