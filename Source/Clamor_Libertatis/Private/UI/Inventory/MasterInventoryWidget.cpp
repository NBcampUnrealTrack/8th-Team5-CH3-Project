#include "UI/Inventory/MasterInventoryWidget.h"
#include "UI/Inventory/QuickSlotWidget.h"
#include "Item/Inventory/ConsumableInventoryComponent.h"
#include "Item/Inventory/SocketItemInventoryComponent.h"
#include "Item/ItemTableRow.h"
#include "Combat/CombatComponent.h"
#include "Combat/HealthComponent.h"
#include "Character/BasePlayerController.h"
#include "Character/PlayerCharacter.h"
#include "Combat/Weapon/WeaponBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UMasterInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BackButton)
        BackButton->OnClicked.AddDynamic(
            this, &UMasterInventoryWidget::OnBackButtonClicked);

    AllItemSlots = {
        QuickSlot1, QuickSlot2, QuickSlot3, QuickSlot4,
        InvSlot5,   InvSlot6,   InvSlot7,   InvSlot8,
        InvSlot9,   InvSlot10,  InvSlot11,  InvSlot12
    };

    for (int32 i = 0; i < AllItemSlots.Num(); i++)
    {
        if (!AllItemSlots[i]) continue;
        AllItemSlots[i]->SlotIndex = i;
        AllItemSlots[i]->bIsInventorySlot = true;
    }

    if (QuickSlot1) { QuickSlot1->bIsInventorySlot = false; QuickSlot1->SetKeyText("1"); QuickSlot1->SetMasterWidget(this); }
    if (QuickSlot2) { QuickSlot2->bIsInventorySlot = false; QuickSlot2->SetKeyText("2"); QuickSlot2->SetMasterWidget(this); }
    if (QuickSlot3) { QuickSlot3->bIsInventorySlot = false; QuickSlot3->SetKeyText("3"); QuickSlot3->SetMasterWidget(this); }
    if (QuickSlot4) { QuickSlot4->bIsInventorySlot = false; QuickSlot4->SetKeyText("4"); QuickSlot4->SetMasterWidget(this); }

    AllSocketItemSlots = {
        SocketItemSlot1, SocketItemSlot2, SocketItemSlot3,
        SocketItemSlot4, SocketItemSlot5
    };

    for (int32 i = 0; i < AllSocketItemSlots.Num(); i++)
    {
        if (!AllSocketItemSlots[i]) continue;
        AllSocketItemSlots[i]->SlotIndex = i;
    }

    if (ItemInfoPanel)
        ItemInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
    if (ConsumableItemInfoPanel)
        ConsumableItemInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UMasterInventoryWidget::InitWidget(
    UConsumableInventoryComponent* InItemInventory,
    USocketItemInventoryComponent* InSocketInventory,
    UQuickSlotWidget* InQuickSlotWidget,
    AWeaponBase* InWeapon)
{
    ItemInventoryComp = InItemInventory;
    SocketInventoryComp = InSocketInventory;
    QuickSlotWidgetRef = InQuickSlotWidget;
    WeaponRef = InWeapon;

    for (UQuickSlotSlotWidget* SlotWidget : AllItemSlots)
    {
        if (SlotWidget)
        {
            SlotWidget->InitQuickSlotSlot(QuickSlotWidgetRef);
            SlotWidget->SetMasterWidget(this);
        }
    }

    for (USocketItemSlotWidget* SlotWidget : AllSocketItemSlots)
    {
        if (SlotWidget)
            SlotWidget->InitSlot(this, SocketInventoryComp);
    }

    if (BladeSocketSlot)
    {
        BladeSocketSlot->InitSocketSlot(WeaponRef, EWeaponSocketType::Blade);
        BladeSocketSlot->SetInventoryComp(SocketInventoryComp);
        BladeSocketSlot->SetMasterWidget(this);
    }
    if (GripSocketSlot)
    {
        GripSocketSlot->InitSocketSlot(WeaponRef, EWeaponSocketType::Grip);
        GripSocketSlot->SetInventoryComp(SocketInventoryComp);
        GripSocketSlot->SetMasterWidget(this);
    }

    if (ItemInventoryComp && !ItemInventoryComp->OnInventoryChanged.IsAlreadyBound(
        this, &UMasterInventoryWidget::OnItemInventoryChanged))
    {
        ItemInventoryComp->OnInventoryChanged.AddDynamic(
            this, &UMasterInventoryWidget::OnItemInventoryChanged);
    }

    if (SocketInventoryComp && !SocketInventoryComp->OnInventoryChanged.IsAlreadyBound(
        this, &UMasterInventoryWidget::OnSocketInventoryChanged))
    {
        SocketInventoryComp->OnInventoryChanged.AddDynamic(
            this, &UMasterInventoryWidget::OnSocketInventoryChanged);
    }

    if (ItemInfoPanel)
        ItemInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
    if (ConsumableItemInfoPanel)
        ConsumableItemInfoPanel->SetVisibility(ESlateVisibility::Collapsed);

    RefreshAll();
}

void UMasterInventoryWidget::RefreshAll()
{
    RefreshItemInventory();
    RefreshSocketInventory();
    RefreshStats();
}

void UMasterInventoryWidget::RefreshItemInventory()
{
    if (!ItemInventoryComp) return;

    for (int32 i = 0; i < AllItemSlots.Num(); i++)
    {
        UQuickSlotSlotWidget* SlotWidget = AllItemSlots[i];
        if (!SlotWidget) continue;

        if (!ItemInventoryComp->Slots.IsValidIndex(i) ||
            ItemInventoryComp->Slots[i].IsEmpty())
        {
            SlotWidget->ClearSlot();
            continue;
        }

        const FInventorySlot& InvSlot = ItemInventoryComp->Slots[i];
        UTexture2D* Icon = nullptr;
        FItemTableRow* ItemData = ItemInventoryComp->GetItemData(InvSlot.ItemID);
        if (ItemData && !ItemData->Icon.IsNull())
            Icon = ItemData->Icon.LoadSynchronous();

        SlotWidget->UpdateSlot(InvSlot.ItemID, InvSlot.Quantity, Icon);
    }

    if (QuickSlotWidgetRef)
        QuickSlotWidgetRef->RefreshAllSlots();
}

void UMasterInventoryWidget::RefreshSocketInventory()
{
    if (!SocketInventoryComp) return;

    for (int32 i = 0; i < AllSocketItemSlots.Num(); i++)
    {
        USocketItemSlotWidget* SlotWidget = AllSocketItemSlots[i];
        if (!SlotWidget) continue;

        SlotWidget->SetVisibility(ESlateVisibility::Visible);

        if (!SocketInventoryComp->Slots.IsValidIndex(i) ||
            SocketInventoryComp->Slots[i].IsEmpty())
        {
            SlotWidget->ClearSlot();
            continue;
        }

        const FInventorySlot& InvSlot = SocketInventoryComp->Slots[i];
        UTexture2D* Icon = nullptr;
        FItemTableRow* ItemData = SocketInventoryComp->GetItemData(InvSlot.ItemID);
        if (ItemData && !ItemData->Icon.IsNull())
            Icon = ItemData->Icon.LoadSynchronous();

        SlotWidget->UpdateSlot(InvSlot.ItemID, Icon);
    }

    if (BladeSocketSlot) BladeSocketSlot->RefreshSlot();
    if (GripSocketSlot) GripSocketSlot->RefreshSlot();
}

void UMasterInventoryWidget::RefreshStats()
{
    APlayerCharacter* PlayerCharacter =
        Cast<APlayerCharacter>(GetOwningPlayerPawn());
    if (!PlayerCharacter) return;

    UHealthComponent* HealthComp = PlayerCharacter->GetHealthComponent();
    if (HealthComp)
    {
        if (CurrentHPText)
            CurrentHPText->SetText(FText::FromString(FString::Printf(
                TEXT("%d / %d"),
                FMath::FloorToInt(HealthComp->GetCurrentHealth()),
                FMath::FloorToInt(HealthComp->GetMaxHealth()))));

        if (CurrentStaminaText)
            CurrentStaminaText->SetText(FText::FromString(FString::Printf(
                TEXT("%d / %d"),
                FMath::FloorToInt(HealthComp->GetCurrentStamina()),
                FMath::FloorToInt(HealthComp->GetMaxStamina()))));

        if (CurrentManaText)
            CurrentManaText->SetText(FText::FromString(FString::Printf(
                TEXT("%d / %d"),
                FMath::FloorToInt(HealthComp->GetCurrentMana()),
                FMath::FloorToInt(HealthComp->GetMaxMana()))));
    }

    UCombatComponent* CombatComp =
        PlayerCharacter->FindComponentByClass<UCombatComponent>();
    if (CombatComp && AttackDamageText)
        AttackDamageText->SetText(FText::AsNumber(
            FMath::FloorToInt(CombatComp->GetBaseAttackDamage())));

    if (MoveSpeedText)
        MoveSpeedText->SetText(FText::AsNumber(
            FMath::FloorToInt(
                PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed)));

    if (ItemInventoryComp && InventoryCountText)
    {
        int32 FilledSlots = 0;
        for (const FInventorySlot& InvSlot : ItemInventoryComp->Slots)
        {
            if (!InvSlot.IsEmpty()) FilledSlots++;
        }
        InventoryCountText->SetText(FText::FromString(FString::Printf(
            TEXT("%d / %d"),
            FilledSlots,
            ItemInventoryComp->Slots.Num())));
    }
}

void UMasterInventoryWidget::UnequipSocketItemToInventory(FName ItemID)
{
    if (!WeaponRef) return;

    for (EWeaponSocketType Type : {
        EWeaponSocketType::Blade, EWeaponSocketType::Grip })
    {
        UWeaponSocketItemData* EquippedItem =
            WeaponRef->GetEquippedSocketItem(Type);
        if (EquippedItem && EquippedItem->InventoryItemID == ItemID)
        {
            WeaponRef->UnequipSocketItem(Type);
            if (BladeSocketSlot) BladeSocketSlot->RefreshSlot();
            if (GripSocketSlot) GripSocketSlot->RefreshSlot();
            RefreshStats();
            return;
        }
    }
}
void UMasterInventoryWidget::ShowItemInfo(const FName& ItemID)
{
    if (!ItemInfoPanel || !SocketInventoryComp) return;

    FItemTableRow* ItemData = SocketInventoryComp->GetItemData(ItemID);
    if (!ItemData) return;

    ItemInfoPanel->ShowInfo(*ItemData);
}

void UMasterInventoryWidget::HideItemInfo()
{
    if (ItemInfoPanel)
        ItemInfoPanel->HideInfo();
}

void UMasterInventoryWidget::ShowConsumableItemInfo(const FName& ItemID)
{
    if (!ConsumableItemInfoPanel || !ItemInventoryComp) return;

    FItemTableRow* ItemData = ItemInventoryComp->GetItemData(ItemID);
    if (!ItemData) return;

    ConsumableItemInfoPanel->InitPopup(*ItemData);
    ConsumableItemInfoPanel->SetVisibility(ESlateVisibility::Visible);
}

void UMasterInventoryWidget::HideConsumableItemInfo()
{
    if (ConsumableItemInfoPanel)
        ConsumableItemInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UMasterInventoryWidget::OnItemInventoryChanged()
{
    RefreshItemInventory();
    RefreshStats();
}

void UMasterInventoryWidget::OnSocketInventoryChanged()
{
    RefreshSocketInventory();
}

void UMasterInventoryWidget::OnBackButtonClicked()
{
    ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwningPlayer());
    if (!PC) return;
    PC->HideMasterInventory();
}