#include "Enemy/E_Weapon/Enemy_BaseWeapon.h"



AEnemy_BaseWeapon::AEnemy_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(SceneComp);
	StaticMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}


void AEnemy_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AEnemy_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

