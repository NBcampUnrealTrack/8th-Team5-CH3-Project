#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UEnemy_StatComponent;
class AEnemy_BaseWeapon;

UCLASS()
class CLAMOR_LIBERTATIS_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	
	FORCEINLINE UEnemy_StatComponent* GetEnemyStatComp() const {return Enemy_StatComp;}
	
	virtual void AttackToPlayer();
protected:
	virtual void BeginPlay() override;
	
	void EquipWeapon();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	TSubclassOf<AActor> Enemy_WeaponClass;
	UPROPERTY()
	TObjectPtr<AEnemy_BaseWeapon> Enemy_WeaponInst;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="ActorComponent")
	TObjectPtr<UEnemy_StatComponent> Enemy_StatComp;
public:
	virtual void Tick(float DeltaTime) override;
};
