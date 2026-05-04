#include "Enemy/AIController/Enemy_AIController.h"



AEnemy_AIController::AEnemy_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

