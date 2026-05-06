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
