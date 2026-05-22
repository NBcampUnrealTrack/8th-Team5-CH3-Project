#include "Enemy/Animations/BaseEnemyAnimInst.h"

#include "Enemy/BaseEnemy.h"
#include "KismetAnimationLibrary.h"

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
	FRotator OwnerRotation = MyOwnerInst->GetActorRotation();
	if (Enemy_GroundSpeed > KINDA_SMALL_NUMBER)
		Enemy_RotationYaw = UKismetAnimationLibrary::CalculateDirection(Velocity, OwnerRotation);
	else
		Enemy_RotationYaw = 0.f;
}