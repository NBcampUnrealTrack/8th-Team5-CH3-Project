// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "WeaponTrail_TimedNiagara.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class USkeletalMeshComponent;
class UAnimSequenceBase;
class UFXSystemComponent;

UCLASS()
class CLAMOR_LIBERTATIS_API UWeaponTrail_TimedNiagara : public UAnimNotifyState_TimedNiagaraEffect
{
	GENERATED_BODY()
public:
	UWeaponTrail_TimedNiagara();

protected:
	virtual UFXSystemComponent* SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const override;

private:
	UNiagaraSystem* GetWeaponTrailNiagara(USkeletalMeshComponent* MeshComp) const;
};
