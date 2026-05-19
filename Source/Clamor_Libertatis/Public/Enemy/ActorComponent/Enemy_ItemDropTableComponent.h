#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_ItemDropTableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UEnemy_ItemDropTableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemy_ItemDropTableComponent();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	TObjectPtr<UDataTable> DT_ItemData;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
