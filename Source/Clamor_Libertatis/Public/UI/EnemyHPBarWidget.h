// EnemyHPBarWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

class UProgressBar;  
class ABaseEnemy;

UCLASS()
class UEnemyHPBarWidget : public UUserWidget
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void UpdateHP(float CurrentHP, float MaxHP);

    void SetTargetEnemy(ABaseEnemy* Enemy);

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;

    TWeakObjectPtr<ABaseEnemy> TargetEnemy;
};