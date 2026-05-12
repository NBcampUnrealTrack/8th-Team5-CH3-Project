#pragma once

#include "CoreMinimal.h"
#include "Item/ItemEffectHandler.h"
#include "HealEffectHandler.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API UHealEffectHandler : public UItemEffectHandler
{
	GENERATED_BODY()

public:
	virtual bool Execute_Implementation(AActor* Owner, const FItemTableRow& ItemData) override;
};
	
