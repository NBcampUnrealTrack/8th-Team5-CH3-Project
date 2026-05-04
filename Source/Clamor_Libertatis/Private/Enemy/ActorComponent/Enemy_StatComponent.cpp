#include "Enemy/ActorComponent/Enemy_StatComponent.h"



UEnemy_StatComponent::UEnemy_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UEnemy_StatComponent::BeginPlay()
{
	Super::BeginPlay();
}



void UEnemy_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

