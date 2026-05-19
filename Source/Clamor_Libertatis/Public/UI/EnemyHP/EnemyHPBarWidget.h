#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
class ABaseEnemy;

UCLASS()
class UEnemyHPBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void UpdateHP(float CurrentHP, float MaxHP);
    void SetTargetEnemy(ABaseEnemy* Enemy);
    FORCEINLINE ABaseEnemy* GetTargetEnemy() const { return TargetEnemy.Get(); }

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> EnemyNameText;

    TWeakObjectPtr<ABaseEnemy> TargetEnemy;
};