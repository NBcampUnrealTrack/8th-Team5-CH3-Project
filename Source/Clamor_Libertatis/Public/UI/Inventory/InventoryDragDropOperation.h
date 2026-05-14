#pragma once
#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Item/ItemTableRow.h"
#include "InventoryDragDropOperation.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UInventoryDragDropOperation : public UDragDropOperation
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    FName ItemID = NAME_None;

    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    int32 SourceInventorySlotIndex = -1;

    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    int32 SourceHotbarSlotIndex = -1;
};