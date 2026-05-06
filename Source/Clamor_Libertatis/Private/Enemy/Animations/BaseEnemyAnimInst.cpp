#include "Enemy/Animations/BaseEnemyAnimInst.h"

#include "Enemy/BaseEnemy.h"

void UBaseEnemyAnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	MyOwnerInst = Cast<ABaseEnemy>(TryGetPawnOwner());
}

void UBaseEnemyAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!MyOwnerInst)
	{
		MyOwnerInst = Cast<ABaseEnemy>(TryGetPawnOwner());
	}
	
	FVector Velocity = MyOwnerInst->GetVelocity();
	Enemy_GroundSpeed = Velocity.Size2D();
}

void UBaseEnemyAnimInst::PlayAM_Attack_Sweep()
{
	UE_LOG(LogTemp,Warning,TEXT("Play AnimMontage Attack Sweep"));
	Montage_Play(AM_Attack_Sweep);
}

void UBaseEnemyAnimInst::PlayAM_JumpAttack()
{
	UE_LOG(LogTemp,Warning,TEXT("Play AnimMontage JumpAttack"));
	Montage_Play(AM_JumpAttack);
}

void UBaseEnemyAnimInst::PlayAM_SingleAttack()
{
	UE_LOG(LogTemp,Warning,TEXT("Play AnimMontage SingleAttack"));
	Montage_Play(AM_SingleAttack);
}

void UBaseEnemyAnimInst::PlayAM_3ComboAttack()
{
	UE_LOG(LogTemp,Warning,TEXT("Play AnimMontage 3ComboAttack"));
	Montage_Play(AM_3ComboAttack);
}
