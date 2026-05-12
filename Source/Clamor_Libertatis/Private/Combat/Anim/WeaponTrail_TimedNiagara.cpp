// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Anim/WeaponTrail_TimedNiagara.h"
#include "Combat/CombatComponent.h"
#include "Combat/Weapon/WeaponBase.h"
#include "Animation/AnimSequenceBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

UWeaponTrail_TimedNiagara::UWeaponTrail_TimedNiagara()
{
	SocketName = TEXT("Hand_R_Weapon");
	bDestroyAtEnd = false;
}

UFXSystemComponent* UWeaponTrail_TimedNiagara::SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const
{
	UNiagaraSystem* NiagaraTemplate = GetWeaponTrailNiagara(MeshComp);
	if (!NiagaraTemplate)
	{
		NiagaraTemplate = Template;
	}

	if (!MeshComp || !NiagaraTemplate)
	{
		return nullptr;
	}

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraTemplate,
		MeshComp,
		SocketName,
		LocationOffset,
		RotationOffset,
		EAttachLocation::KeepRelativeOffset,
		true,
		true,
		ENCPoolMethod::None,
		true
	);

	if (NiagaraComponent)
	{
		NiagaraComponent->ComponentTags.AddUnique(GetSpawnedComponentTag());
		if (bApplyRateScaleAsTimeDilation && Animation)
		{
			NiagaraComponent->SetCustomTimeDilation(Animation->RateScale);
		}
	}

	return NiagaraComponent;
}

UNiagaraSystem* UWeaponTrail_TimedNiagara::GetWeaponTrailNiagara(USkeletalMeshComponent* MeshComp) const
{
	if (!MeshComp)
	{
		return nullptr;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return nullptr;
	}

	const UCombatComponent* CombatComponent = OwnerActor->FindComponentByClass<UCombatComponent>();
	if (!CombatComponent)
	{
		return nullptr;
	}

	const AWeaponBase* CurrentWeapon = CombatComponent->GetCurrentWeapon();
	return CurrentWeapon ? CurrentWeapon->GetWeaponTrailNiagara() : nullptr;
}
