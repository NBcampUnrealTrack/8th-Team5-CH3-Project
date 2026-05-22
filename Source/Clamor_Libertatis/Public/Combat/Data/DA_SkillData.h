// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Combat/DamageType/CustomDamageType.h"
#include "DA_SkillData.generated.h"

class AActor;
class UAnimMontage;
class UNiagaraSystem;
class USoundBase;

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Melee,
	Projectile,
	Area,
	Buff,
	Summon
};

UENUM(BlueprintType)
enum class ESkillEventType : uint8
{
	Attack,
	Effect,
	Projectile,
	Spawn,
	Sound
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FSkillEventCommonData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	float Radius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	float Range = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FName SpawnSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	TSubclassOf<UDamageType> DamageTypeClass = UMagicDamageType::StaticClass();
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FSkillAttackEventData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Attack")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Pawn;
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FSkillEffectEventData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Effect")
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Effect")
	FVector Scale = FVector(1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Effect")
	bool bAttachToSocket = false;
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FSkillProjectileEventData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Projectile")
	float ExplosionRadius = 0.f;
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FSkillSpawnEventData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Spawn")
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Spawn")
	bool bSpawnAtAimTarget = true;
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FSkillSoundEventData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Sound")
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Sound")
	float VolumeMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event|Sound")
	float PitchMultiplier = 1.f;
};

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FSkillEventData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FName EventID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	ESkillEventType EventType = ESkillEventType::Projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FSkillEventCommonData CommonData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FSkillAttackEventData AttackData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FSkillEffectEventData EffectData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FSkillProjectileEventData ProjectileData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FSkillSpawnEventData SpawnData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	FSkillSoundEventData SoundData;
};

UCLASS()
class CLAMOR_LIBERTATIS_API UDA_SkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("SkillData"), GetFName());
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FName SkillName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Cost")
	float StaminaCost = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Cost")
	float ManaCost = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Cooldown")
	float Cooldown = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill|Animation")
	TObjectPtr<UAnimMontage> CastMontage; //시전동작

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Event")
	TArray<FSkillEventData> Events;
};
