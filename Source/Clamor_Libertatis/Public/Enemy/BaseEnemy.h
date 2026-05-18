#pragma once

#include "CoreMinimal.h"
#include "DataTable/DA_BaseEnemySkill.h"
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
	FORCEINLINE AEnemy_BaseWeapon* GetEnemyWeapon() const { return Enemy_WeaponInst; }
	FORCEINLINE bool IsDead() const { return bIsDead; }

	virtual UAnimMontage* AttackToPlayer();

	UFUNCTION(BlueprintCallable)
	virtual void AttackHitCheck();

	UFUNCTION(BlueprintCallable)
	void ApplyStun(float Duration);

	const FEnemySkillInfo* GetCurrentSkillInfo() const;

private:
	UFUNCTION()
	void EndStun();

	FTimerHandle StunTimerHandle;
	bool bIsStunned = false;

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	virtual void OnDead();
	
	void EquipWeapon();
	virtual float GetCurrentAttackDamage() const;
	void BeginMontageAttack(UAnimMontage* Montage);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
#pragma region Weapon
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	TSubclassOf<AActor> Enemy_WeaponClass;
	UPROPERTY()
	TObjectPtr<AEnemy_BaseWeapon> Enemy_WeaponInst;
#pragma endregion 
#pragma region ActorComp
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="ActorComponent")
	TObjectPtr<UEnemy_StatComponent> Enemy_StatComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="ActorComponent")
	TObjectPtr<UEnemy_CombatComponent> Enemy_CombatComp;
#pragma endregion 
#pragma region Animations
	UPROPERTY()
	TObjectPtr<UBaseEnemyAnimInst> AnimInst;
#pragma endregion

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<AActor> DamageTextActorClass;
	
	TPair<EAttackType,int32> CurrentAttackData;
	bool bIsDead;
	
public:
	virtual void Tick(float DeltaTime) override;
};
