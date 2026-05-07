#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_CombatComponent();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
