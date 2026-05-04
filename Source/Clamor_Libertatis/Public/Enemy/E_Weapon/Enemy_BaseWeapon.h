#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy_BaseWeapon.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API AEnemy_BaseWeapon : public AActor
{
	GENERATED_BODY()
public:
	AEnemy_BaseWeapon();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};
