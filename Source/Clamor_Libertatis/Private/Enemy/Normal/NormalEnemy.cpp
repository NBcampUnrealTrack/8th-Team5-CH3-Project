#include "Enemy/Normal/NormalEnemy.h"



ANormalEnemy::ANormalEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANormalEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
}


void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}


void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

