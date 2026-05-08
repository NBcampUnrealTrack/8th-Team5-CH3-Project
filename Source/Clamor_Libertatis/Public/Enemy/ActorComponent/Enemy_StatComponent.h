#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, float, CurrentHP, float, MaxHP);

USTRUCT()
struct FEnemyStat
{
	GENERATED_BODY()
public:
	float HP;
	float MaxHP;
	float Attack_Damage;
	float SightRadius;
	float LoseSightRadius;
	float PeripheralVisionAngleDegrees;
	float SightConfig_MaxAge;
	
	bool SetHP(float DamageAmount)
	{
		HP = FMath::Clamp(HP - DamageAmount, 0.f, MaxHP);
		if (HP <= 0.f)
		{
			return false;
		}
		UE_LOG(LogTemp,Warning,TEXT("Current HP : %f"),HP);
		return true;
	}
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_StatComponent();
	
	void InitializeStat();
	FORCEINLINE FEnemyStat& GetEnemyStat() {return EnemyStat;}

	UPROPERTY(BlueprintAssignable)
	FOnHPChanged OnHPChanged;

protected:
	virtual void BeginPlay() override;
	
#pragma region EnemyStat
	UPROPERTY()
	FEnemyStat EnemyStat;
#pragma endregion 
#pragma region DataTable
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	FDataTableRowHandle DT_EnemyRowHandle;
#pragma endregion 
	
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
