#pragma once
#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UInventoryDragDropOperation : public UDragDropOperation
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Category = "DragDrop")
    FName ItemID;
};