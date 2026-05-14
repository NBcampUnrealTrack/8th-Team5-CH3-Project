// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Weapon/WeaponAttackData.h"
#include "Combat/Weapon/WeaponSocketItemData.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UAnimMontage;
class ACharacter;
class UPrimitiveComponent;
struct FHitResult;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FWeaponSocketSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Socket")
	EWeaponSocketType SocketTag = EWeaponSocketType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Socket")
	TObjectPtr<UWeaponSocketItemData> EquippedItem;

	bool IsEmpty() const { return EquippedItem == nullptr; }
};

UCLASS()
class CLAMOR_LIBERTATIS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

public:
	UAnimMontage* GetAttackMontage() const;

	int32 GetMaxComboCount() const;

	FName GetAttackSectionName(int32 ComboIndex) const;

	const FWeaponAttackData* GetAttackData(int32 ComboIndex) const;

	float GetAttackDamage(int32 ComboIndex) const;
	float GetAttackStaminaCost(int32 ComboIndex) const;
	float GetManaCostMultiplier() const;
	float GetAttackSpeedMultiplier() const;

	void EnableHitbox();
	void DisableHitbox();
	

	void AttachToCharacterHand(ACharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon|Socket")
	bool EquipSocketItem(UWeaponSocketItemData* SocketItem, EWeaponSocketType SocketTag);

	UFUNCTION(BlueprintCallable, Category="Weapon|Socket")
	UWeaponSocketItemData* UnequipSocketItem(EWeaponSocketType SocketTag);

	UFUNCTION(BlueprintPure, Category="Weapon|Socket")
	bool CanEquipSocketItem(const UWeaponSocketItemData* SocketItem, EWeaponSocketType SocketTag) const;

	UFUNCTION(BlueprintPure, Category="Weapon|Socket")
	UWeaponSocketItemData* GetEquippedSocketItem(EWeaponSocketType SocketTag) const;

	UFUNCTION(BlueprintPure, Category="Weapon|Socket")
	TArray<FWeaponSocketSlot> GetSocketSlots() const { return SocketSlots; }

	UFUNCTION(BlueprintCallable, Category="Weapon|VFX")
	void SetWeaponTrailNiagara(UNiagaraSystem* NewTrailNiagara);

	UFUNCTION(BlueprintPure, Category="Weapon|VFX")
	UNiagaraSystem* GetWeaponTrailNiagara() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	UBoxComponent* Hitbox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	TSet<AActor*> HitActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Attack")
	UAnimMontage* AttackComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Attack")
	TArray<FWeaponAttackData> AttackDataList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Socket")
	TArray<EWeaponSocketType> WeaponTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|Socket")
	TArray<FWeaponSocketSlot> SocketSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon|VFX")
	TObjectPtr<UNiagaraSystem> TrailNiagara;
private:
	void InitializeDefaultSocketSlots();

	FWeaponSocketSlot* FindSocketSlot(EWeaponSocketType SocketTag);

	const FWeaponSocketSlot* FindSocketSlot(EWeaponSocketType SocketTag) const;

	TArray<const UWeaponSocketItemData*> GetEquippedSocketItems() const;

	UFUNCTION()
	void OnHitboxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
