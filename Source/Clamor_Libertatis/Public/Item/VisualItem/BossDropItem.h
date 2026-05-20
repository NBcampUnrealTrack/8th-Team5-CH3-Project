#pragma once

#include "CoreMinimal.h"
#include "Item/VisualItem/DropItem.h"
#include "Item/ItemTableRow.h"
#include "BossDropItem.generated.h"

class UNiagaraComponent;

USTRUCT(BlueprintType)
struct FBossDropEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	FDataTableRowHandle RowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop", meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float DropChance = 0.5f;
};

UCLASS()
class CLAMOR_LIBERTATIS_API ABossDropItem : public ADropItem
{
	GENERATED_BODY()

public:
	ABossDropItem();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraComponent> NiagaraComp;

	// 에디터에서 드랍 가능한 아이템과 각 확률을 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	TArray<FBossDropEntry> DropTable;

	// 확률 롤 결과 - 실제로 드랍된 아이템 목록
	TArray<FItemTableRow*> RolledItems;
	
	FBossDropEntry BossDropItemRowHandle;

	virtual void HandleItemOverlap(AActor* OtherActor) override;

private:
	void RollDropItems();
};