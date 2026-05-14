#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class CLAMOR_LIBERTATIS_API UStatWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void InitStatWidget(APlayerController* InController);

    UFUNCTION(BlueprintCallable)
    void RefreshStats();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentHPText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentStaminaText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentManaText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* AttackDamageText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MoveSpeedText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* InventoryCountText;

    UPROPERTY(meta = (BindWidget))
    UButton* InventoryButton;
    // UPROPERTY(meta = (BindWidget))
    // UTextBlock* StageNumberText; // 스테이지 구현 후 주석 해제

    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;   
    
private:
    UPROPERTY()
    TObjectPtr<APlayerController> OwnerController;

    UFUNCTION()
    void OnBackButtonClicked();     
    UFUNCTION()
    void OnInventoryButtonClicked();
};