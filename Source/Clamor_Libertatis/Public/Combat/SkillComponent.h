// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class ACharacter;
class UDA_SkillData;
class UHealthComponent;


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
	void ExecutePendingSkill();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	TObjectPtr<UDA_SkillData> DefaultSkillData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Skill")
	FName SkillSpawnSocketName = TEXT("Hand_R_Weapon");

private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY()
	TObjectPtr<UDA_SkillData> PendingSkillData;

	float SkillCooldownEndTime = 0.0f;

	bool CanActivateSkill(const UDA_SkillData* SkillData) const;
	bool CommitSkillCost(const UDA_SkillData* SkillData) const;
	void StartCooldown(const UDA_SkillData* SkillData);
	void ExecuteSkill(const UDA_SkillData* SkillData);

	void SpawnProjectileSkill(const UDA_SkillData* SkillData);

	FVector GetAimTargetLocation(const UDA_SkillData* SkillData) const;
	FVector GetSkillSpawnLocation() const;

};
