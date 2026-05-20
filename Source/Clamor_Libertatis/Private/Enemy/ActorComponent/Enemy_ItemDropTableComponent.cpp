#include "Enemy/ActorComponent/Enemy_ItemDropTableComponent.h"



UEnemy_ItemDropTableComponent::UEnemy_ItemDropTableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemy_ItemDropTableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemy_ItemDropTableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UEnemy_ItemDropTableComponent::DropItem()
{
	
}