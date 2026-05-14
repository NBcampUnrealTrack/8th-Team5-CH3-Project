#include "Enemy/Boss/BossEnemy_Mage.h"



ABossEnemy_Mage::ABossEnemy_Mage()
{
	
	PrimaryActorTick.bCanEverTick = true;
}


void ABossEnemy_Mage::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABossEnemy_Mage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ABossEnemy_Mage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

