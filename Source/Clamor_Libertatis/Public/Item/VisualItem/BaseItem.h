#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemTableRow.h"
#include "BaseItem.generated.h"


UCLASS()
class CLAMOR_LIBERTATIS_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DataTable")
	TObjectPtr<UDataTable> DT_ItemTable;
	
	TArray<FItemTableRow*> ItemTableRowHandle;
public:
	virtual void Tick(float DeltaTime) override;
};
