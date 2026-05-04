#include "Enemy/E_Weapon/Enemy_BaseWeapon.h"



AEnemy_BaseWeapon::AEnemy_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AEnemy_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

