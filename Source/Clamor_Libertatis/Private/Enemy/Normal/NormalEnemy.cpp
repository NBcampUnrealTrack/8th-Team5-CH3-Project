#include "Enemy/Normal/NormalEnemy.h"



ANormalEnemy::ANormalEnemy()
{
	
	PrimaryActorTick.bCanEverTick = true;
}


void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}


void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ANormalEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

