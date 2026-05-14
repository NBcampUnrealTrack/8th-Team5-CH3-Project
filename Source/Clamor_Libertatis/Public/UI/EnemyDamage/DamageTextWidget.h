#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

class UTextBlock;

UCLASS()
class CLAMOR_LIBERTATIS_API UDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void InitDamageText(float Damage);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TB_Damage;
	
};
