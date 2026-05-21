#include "UI/AfterEndingWidget.h"
#include "Components/Button.h"
#include "Character/BasePlayerController.h"
#include "Kismet/KismetSystemLibrary.h"

void UAfterEndingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_Restart)
        Btn_Restart->OnClicked.AddDynamic(
            this, &UAfterEndingWidget::OnRestartClicked);

    if (Btn_Quit)
        Btn_Quit->OnClicked.AddDynamic(
            this, &UAfterEndingWidget::OnQuitClicked);
}

void UAfterEndingWidget::OnRestartClicked()
{
    ABasePlayerController* PC =
        Cast<ABasePlayerController>(GetOwningPlayer());
    if (PC) PC->RestartGame();
}

void UAfterEndingWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(
        this, GetOwningPlayer(),
        EQuitPreference::Quit, false);
}