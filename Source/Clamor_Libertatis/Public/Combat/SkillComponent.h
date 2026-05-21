// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Data/DA_SkillData.h"
#include "SkillComponent.generated.h"

class ACharacter;
class UHealthComponent;
class UAnimMontage;
class UTargetLockComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogSkill, Log, All)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCooldownStart, float, Cooldown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMultiSkillCooldownStart, FName, SkillName , float, Cooldown);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAMOR_LIBERTATIS_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

	UFUNCTION(BlueprintCallable, Category="Skill")
	void ActiveSkill();

	UFUNCTION(BlueprintCallable, Category="Skill")
	bool TryActivateSkill(UDA_SkillData* SkillData);
	
	UFUNCTION(BlueprintCallable, Category="Skill")
	void ExecuteSkillEvent(FName EventID);

	FOnSkillCooldownStart OnSkillCooldownStart;
	FOnMultiSkillCooldownStart OnMultiSkillCooldownStart;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	TObjectPtr<UDA_SkillData> DefaultSkillData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FName SkillSpawnSocketName = TEXT("Hand_R_Weapon");//마법이 나갈 위치 소켓

private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY()
	TObjectPtr<UTargetLockComponent> LockOnComponent;

	UPROPERTY()
	TObjectPtr<UDA_SkillData> PendingSkillData;

	//float SkillCooldownEndTime = 0.0f;

	TMap<FName, float> SkillCooldownList;

	bool CanActivateSkill(const UDA_SkillData* SkillData) const;
	bool CommitSkillCost(const UDA_SkillData* SkillData) const;
	void StartCooldown(const UDA_SkillData* SkillData);
	void ExecuteAllSkillEvents(const UDA_SkillData* SkillData);
	void SetLockOnRotationOverride(bool bEnable) const;

	void ExecuteEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData);

	void ExecuteAttackEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData);
	void PlayEffectEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData);
	void SpawnProjectileEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData);
	void SpawnActorEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData);
	void PlaySoundEvent(const UDA_SkillData* SkillData, const FSkillEventData& EventData);


	FVector GetAimTargetLocation(float Range) const;
	FVector GetSkillSpawnLocation() const;
	FVector GetSkillEventLocation(const FSkillEventCommonData& CommonData) const;

	UFUNCTION()
	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};
