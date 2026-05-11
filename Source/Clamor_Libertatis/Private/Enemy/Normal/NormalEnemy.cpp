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

void ANormalEnemy::OnDead()
{
	Super::OnDead();
}

float ANormalEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	//TODO:: 경직로직
	
	
	
	return ActualDamage;
}

void ANormalEnemy::AttackHitCheck()
{
	Super::AttackHitCheck();
}


void ANormalEnemy::AttackToPlayer()
{
	Super::AttackToPlayer();
}