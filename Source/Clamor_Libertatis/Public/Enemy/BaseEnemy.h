#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UEnemy_CombatComponent;
class UBaseEnemyAnimInst;
class UEnemy_StatComponent;
class AEnemy_BaseWeapon;

UCLASS()
class CLAMOR_LIBERTATIS_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	
	FORCEINLINE UEnemy_StatComponent* GetEnemyStatComp() const {return Enemy_StatComp;}
	FORCEINLINE UEnemy_CombatComponent* GetEnemyCombatComp() const {return Enemy_CombatComp;}
	
	virtual void AttackToPlayer();
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	virtual void OnDead();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackHitCheck();
	
	void EquipWeapon();
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	TSubclassOf<AActor> Enemy_WeaponClass;
	UPROPERTY()
	TObjectPtr<AEnemy_BaseWeapon> Enemy_WeaponInst;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="ActorComponent")
	TObjectPtr<UEnemy_StatComponent> Enemy_StatComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="ActorComponent")
	TObjectPtr<UEnemy_CombatComponent> Enemy_CombatComp;
	UPROPERTY()
	TObjectPtr<UBaseEnemyAnimInst> AnimInst;
public:
	virtual void Tick(float DeltaTime) override;
};
