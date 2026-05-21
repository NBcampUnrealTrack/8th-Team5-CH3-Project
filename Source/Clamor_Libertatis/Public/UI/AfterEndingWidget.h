#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AfterEndingWidget.generated.h"

class UButton;

UCLASS()
class CLAMOR_LIBERTATIS_API UAfterEndingWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Btn_Restart;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Btn_Quit;

private:
    UFUNCTION()
    void OnRestartClicked();

    UFUNCTION()
    void OnQuitClicked();
};