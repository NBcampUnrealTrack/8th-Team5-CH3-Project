#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/DataTable/DA_BaseEnemyAnim.h"
#include "Enemy_CombatComponent.generated.h"


class UDA_EnemyAttackCollision;
class UDA_BaseEnemyAnim;
struct FBaseEnemyStat;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_CombatComponent();
	
	FORCEINLINE int64 GetCurrentAMCount() const {return DA_EnemyAnim->Map_AM_Battle.Num();}
	FORCEINLINE UAnimMontage* GetAnimMontage(EAnimMontage AnimMontage) const {return DA_EnemyAnim->Map_AM_Battle.FindRef(AnimMontage);}
	FORCEINLINE float GetAttackDistance(EAnimMontage CurrentAnimMontage) const;
	
	FCollisionShape MakeAttackCollision(EAnimMontage CurrentMontage);
	
	UPROPERTY()
	TObjectPtr<UDA_BaseEnemyAnim> DA_EnemyAnim;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataAsset|AttackCollision")
	TObjectPtr<UDA_EnemyAttackCollision> DA_AttackCollision;
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
