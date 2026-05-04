#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_StatComponent.generated.h"


USTRUCT()
struct FEnemyStat
{
	GENERATED_BODY()
	
	float HP;
	float Attack_Damage;
	float SightRadius;
	float LoseSightRadius;
	float PeripheralVisionAngleDegrees;
	float SightConfig_MaxAge;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_StatComponent();
	
	void InitializeStat();
	FORCEINLINE const FEnemyStat& GetEnemyStat() const {return EnemyStat;}
protected:
	virtual void BeginPlay() override;
	
#pragma region EnemyStat
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
