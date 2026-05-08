// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Weapon/WeaponBase.h"
#include "Combat/Weapon/WeaponAttackData.h"
#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Hitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	Hitbox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Hitbox->SetGenerateOverlapEvents(false);
	Hitbox->SetupAttachment(StaticMeshComponent);
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnHitboxBeginOverlap);
	//디버그용
	//Hitbox->ShapeColor = FColor::Green;
	//Hitbox->SetLineThickness(3.f);
	//Hitbox->SetHiddenInGame(true);


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

	return AttackData->Damage;
}

float AWeaponBase::GetAttackStaminaCost(int32 ComboIndex) const
{
	const FWeaponAttackData* AttackData = GetAttackData(ComboIndex);

	if (!AttackData)
	{
		return 0.0f;
	}

	return AttackData->StaminaCost;
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
	Hitbox->SetHiddenInGame(false);
	
}

void AWeaponBase::DisableHitbox()
{
	if (!Hitbox)
	{
		return;
	}

	Hitbox->SetGenerateOverlapEvents(false);
	Hitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Hitbox->SetHiddenInGame(true);
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
	HitActors.Add(OtherActor);

	const float Damage = OwnerCombatComponent->GetCurrentAttackDamage();
	if (Damage <= 0.0f)
	{
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(OwnerActor);
	AController* InstigatorController = OwnerPawn ? OwnerPawn->GetController() : nullptr;

	UGameplayStatics::ApplyDamage(
		OtherActor,
		Damage,
		InstigatorController,
		OwnerActor,
		UDamageType::StaticClass()
	);
}
