// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLAMOR_LIBERTATIS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	UFUNCTION()
	void BasicAttack();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> BasicAttackAnimMontage;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartAttack();
	UFUNCTION()
	void EndAttack(FName NotifyName);
	UFUNCTION()
	void EnableCombo();
	UFUNCTION()
	void CloseCombo();
	UFUNCTION()
	void CheckCombo(FName NotifyName);

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	//아래로 데이터 에셋 등으로 뺄것.
	bool bIsAttacking = false;
	bool bIsComboEnabled = false;
	bool bComboInputBuffered = false;
	int32 ComboIndex = 0;
	int32 MaxCombo = 3;

};
