#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_ItemDropTableComponent.generated.h"

class ABossDropItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_ItemDropTableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_ItemDropTableComponent();

	void DropItem(FVector SpawnLocation);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Drop")
	TSubclassOf<ABossDropItem> BossDropItemClass;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
