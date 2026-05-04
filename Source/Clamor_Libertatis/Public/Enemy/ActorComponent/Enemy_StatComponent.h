#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_StatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_StatComponent();
	
	void InitializeEnemyStat();
	FORCEINLINE float GetEnemyHP() const {return HP;}
	FORCEINLINE float GetEnemyAttack_Damage() const {return Attack_Damage;}
protected:
	virtual void BeginPlay() override;
	
#pragma region EnemyStat
	float HP;
	float Attack_Damage;
#pragma endregion 
#pragma region DataTable
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	FDataTableRowHandle DT_EnemyRowHandle;
#pragma endregion 
	
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
