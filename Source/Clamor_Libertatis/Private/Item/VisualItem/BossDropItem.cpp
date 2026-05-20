#include "Item/VisualItem/BossDropItem.h"
#include "NiagaraComponent.h"

ABossDropItem::ABossDropItem()
{
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComp->SetupAttachment(SceneComp);
}

void ABossDropItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// DT_ItemTable을 RowHandle에 자동 동기화 → 에디터에서 Row 드롭다운 활성화
	if (DT_ItemTable)
	{
		for (FBossDropEntry& Entry : DropTable)
		{
			Entry.RowHandle.DataTable = DT_ItemTable;
		}
	}
}

void ABossDropItem::BeginPlay()
{
	Super::BeginPlay();
	RollDropItems();
}

void ABossDropItem::RollDropItems()
{
	if (!DT_ItemTable)
	{
		// UE_LOG(LogTemp, Warning, TEXT("[BossDropItem] DT_ItemTable is null"));
		return;
	}

	// UE_LOG(LogTemp, Warning, TEXT("[BossDropItem] RollDropItems - DropTable count: %d"), DropTable.Num());

	for (const FBossDropEntry& Entry : DropTable)
	{
		if (Entry.RowHandle.RowName.IsNone())
		{
			// UE_LOG(LogTemp, Warning, TEXT("[BossDropItem] RowName is None, skipping entry"));
			continue;
		}

		float Roll = FMath::FRand();
		FItemTableRow* Row = DT_ItemTable->FindRow<FItemTableRow>(Entry.RowHandle.RowName, TEXT("BossDropItem"));

		// UE_LOG(LogTemp, Warning, TEXT("[BossDropItem] Row [%s] | Roll: %.2f / Chance: %.2f | FindRow: %s"),
		// 	*Entry.RowHandle.RowName.ToString(), Roll, Entry.DropChance, Row ? TEXT("OK") : TEXT("FAIL"));

		if (Roll <= Entry.DropChance && Row)
		{
			RolledItems.Add(Row);
			UE_LOG(LogTemp, Warning, TEXT("[BossDropItem] Added to RolledItems: %s"), *Row->ItemName.ToString());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[BossDropItem] RollDropItems complete - RolledItems count: %d"), RolledItems.Num());
}

void ABossDropItem::HandleItemOverlap(AActor* OtherActor)
{
	if (!OtherActor || RolledItems.IsEmpty()) return;
	
	UE_LOG(LogTemp,Warning,TEXT("Item Overlap"));
	
	// TODO: 팀원이 구현할 인터페이스를 통해 RolledItems 전달
	// IItemReceiverInterface* Receiver = Cast<IItemReceiverInterface>(OtherActor);
	// if (Receiver)
	// {
	//     for (FItemTableRow* Item : RolledItems)
	//         Receiver->ReceiveItem(Item);
	//     Destroy();
	// }
}