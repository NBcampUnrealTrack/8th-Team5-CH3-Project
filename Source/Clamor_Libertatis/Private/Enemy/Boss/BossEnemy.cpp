#include "Enemy/Boss/BossEnemy.h"



ABossEnemy::ABossEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

