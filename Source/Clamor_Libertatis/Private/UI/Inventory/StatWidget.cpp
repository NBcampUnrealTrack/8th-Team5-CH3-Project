#include "UI/Inventory/StatWidget.h"
#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Combat/HealthComponent.h"
#include "Combat/CombatComponent.h"
#include "Item/Inventory/ConsumableInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
// #include "Gamemode/StageGameModeBase.h" // 스테이지 구현 후 주석 해제
// #include "Kismet/GameplayStatics.h"     // 스테이지 구현 후 주석 해제

void UStatWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BackButton)
        BackButton->OnClicked.AddDynamic(this, &UStatWidget::OnBackButtonClicked);

    if (InventoryButton)
        InventoryButton->OnClicked.AddDynamic(this, &UStatWidget::OnInventoryButtonClicked);
}
void UStatWidget::InitStatWidget(APlayerController* InController)
{
    OwnerController = InController;
    RefreshStats();
}

void UStatWidget::RefreshStats()
{
    if (!OwnerController) return;

    APlayerCharacter* PlayerCharacter =
        Cast<APlayerCharacter>(OwnerController->GetPawn());
    if (!PlayerCharacter) return;

    // HP / 스태미나 / 마나
    UHealthComponent* HealthComp = PlayerCharacter->GetHealthComponent();
    if (HealthComp)
    {
        if (CurrentHPText)
            CurrentHPText->SetText(FText::FromString(FString::Printf(
                TEXT("HP : %d / %d"),
                FMath::FloorToInt(HealthComp->GetCurrentHealth()),
                FMath::FloorToInt(HealthComp->GetMaxHealth()))));

        if (CurrentStaminaText)
            CurrentStaminaText->SetText(FText::FromString(FString::Printf(
                TEXT("Stamina : %d / %d"),
                FMath::FloorToInt(HealthComp->GetCurrentStamina()),
                FMath::FloorToInt(HealthComp->GetMaxStamina()))));

        if (CurrentManaText)
            CurrentManaText->SetText(FText::FromString(FString::Printf(
                TEXT("Mana : %d / %d"),
                FMath::FloorToInt(HealthComp->GetCurrentMana()),
                FMath::FloorToInt(HealthComp->GetMaxMana()))));
    }

    // 공격력
    UCombatComponent* CombatComp =
        PlayerCharacter->FindComponentByClass<UCombatComponent>();
    if (CombatComp && AttackDamageText)
        AttackDamageText->SetText(FText::FromString(FString::Printf(
            TEXT("Attack : %d"),
            FMath::FloorToInt(CombatComp->GetBaseAttackDamage()))));

    // 이동속도
    if (MoveSpeedText)
        MoveSpeedText->SetText(FText::FromString(FString::Printf(
            TEXT("Speed : %d"),
            FMath::FloorToInt(PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed))));

    // 인벤토리
    if (PlayerCharacter->ConsumableInventory && InventoryCountText)
    {
        int32 FilledSlots = 0;
        for (const FInventorySlot& InvSlot : PlayerCharacter->ConsumableInventory->Slots)
        {
            if (!InvSlot.IsEmpty()) FilledSlots++;
        }
        InventoryCountText->SetText(FText::FromString(FString::Printf(
            TEXT("Item : %d / %d"),
            FilledSlots,
            PlayerCharacter->ConsumableInventory->Slots.Num())));
    }

    // 스테이지 번호 - 구현 후 주석 해제
    // AStageGameModeBase* GameMode =
    //     Cast<AStageGameModeBase>(UGameplayStatics::GetGameMode(this));
    // if (GameMode && StageNumberText)
    //     StageNumberText->SetText(FText::FromString(FString::Printf(
    //         TEXT("Stage : %d"), GameMode->GetCurrentStage())));
}

void UStatWidget::OnBackButtonClicked()
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(OwnerController);
    if (!PC) return;
    PC->HideStatWidget();
}

void UStatWidget::OnInventoryButtonClicked()
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(OwnerController);
    if (!PC) return;
    PC->HideStatWidget(); 
    PC->ShowInventory();   
}