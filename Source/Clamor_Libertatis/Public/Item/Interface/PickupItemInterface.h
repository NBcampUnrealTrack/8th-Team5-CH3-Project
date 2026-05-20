
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLAMOR_LIBERTATIS_API IPickupItemInterface
{
	GENERATED_BODY()

public:
	virtual bool TryPickupItem(class AActor* Item) = 0;
};
