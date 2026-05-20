#include "Item/VisualItem/BaseItem.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (DT_ItemTable)
	{
		const static FString ContextString = "InitializeItemData"; 
		DT_ItemTable->GetAllRows(ContextString, ItemTableRowHandle);
	}
	for (auto it : ItemTableRowHandle)
	{
		// UE_LOG(LogTemp,Warning,TEXT("Row Name : %s"),*it->ItemName.ToString());
	}
}


void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

