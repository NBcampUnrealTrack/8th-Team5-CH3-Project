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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
public:
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComp() const { return StaticMeshComp; }
};
