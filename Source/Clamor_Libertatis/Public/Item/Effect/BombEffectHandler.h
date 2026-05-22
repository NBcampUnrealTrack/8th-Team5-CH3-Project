#pragma once

#include "CoreMinimal.h"
#include "Item/Effect/ItemEffectHandler.h"
#include "BombEffectHandler.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UBombEffectHandler : public UItemEffectHandler
{
    GENERATED_BODY()
public:
    virtual bool Execute_Implementation(AActor* Owner, const FItemTableRow& ItemData) override;
};