#pragma once

#include "CoreMinimal.h"
#include "DA_BaseEnemyAnim.h"
#include "Engine/DataAsset.h"
#include "DA_EnemyAttackCollision.generated.h"


UENUM()
enum class EAttackCollisionShape : uint8
{
	Box,
	Sphere
};

USTRUCT(BlueprintType)
struct FEnemyAttackCollision
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	EAttackCollisionShape CollisionShape;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	float Distance = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	FVector CollisionSize = FVector::ZeroVector;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	float SphereRadius = 0.f;
};


UCLASS()
class CLAMOR_LIBERTATIS_API UDA_EnemyAttackCollision : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AttackCollision")
	TMap<EAnimMontage,FEnemyAttackCollision> AttackCollisionData;
};
