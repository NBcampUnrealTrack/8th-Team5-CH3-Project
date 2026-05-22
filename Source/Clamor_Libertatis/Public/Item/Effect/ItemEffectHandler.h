#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item/ItemTableRow.h"
#include "ItemEffectHandler.generated.h"

UCLASS(Abstract, Blueprintable)
class CLAMOR_LIBERTATIS_API UItemEffectHandler : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, Category = "Item")
    bool Execute(AActor* Owner, const FItemTableRow& ItemData);
    virtual bool Execute_Implementation(AActor* Owner, const FItemTableRow& ItemData)
    {
        return false;
    }
};