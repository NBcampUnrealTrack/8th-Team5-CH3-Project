#include "Enemy/ActorComponent/Enemy_CombatComponent.h"


UEnemy_CombatComponent::UEnemy_CombatComponent()
{
	DA_EnemyAnim = nullptr;
	PrimaryComponentTick.bCanEverTick = true;
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

