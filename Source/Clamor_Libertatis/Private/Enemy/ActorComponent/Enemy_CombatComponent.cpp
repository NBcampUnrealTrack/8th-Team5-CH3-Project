#include "Enemy/ActorComponent/Enemy_CombatComponent.h"

#include "Enemy/DataTable/DA_EnemyAttackCollision.h"


UEnemy_CombatComponent::UEnemy_CombatComponent()
{
	DA_EnemyAnim = nullptr;
	PrimaryComponentTick.bCanEverTick = true;
}

float UEnemy_CombatComponent::GetAttackDistance(EAnimMontage CurrentAnimMontage) const
{
	float AttackDistance = 0.f;
	if (FEnemyAttackCollision* Data = DA_AttackCollision->AttackCollisionData.Find(CurrentAnimMontage))
	{
		AttackDistance = Data->Distance;
	}
	return AttackDistance;
}

FCollisionShape UEnemy_CombatComponent::MakeAttackCollision(EAnimMontage CurrentMontage)
{
	FCollisionShape CollisionShape;
	
	if (FEnemyAttackCollision* Data = DA_AttackCollision->AttackCollisionData.Find(CurrentMontage))
	{
		FVector CollisionSize = Data->CollisionSize;
		float SphereRadius = Data->SphereRadius;
		
		if (Data->CollisionShape == EAttackCollisionShape::Box)
		{
			CollisionShape = FCollisionShape::MakeBox(CollisionSize);
		}
		else if (Data->CollisionShape == EAttackCollisionShape::Sphere)
		{
			CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
		}
	}
	
	return CollisionShape;
}


void UEnemy_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
}



void UEnemy_CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

