#include "Crafting/W_CraftingWindow.h"
#include "Crafting/W_CraftingSlot.h"
#include "Crafting/W_RecipeSelectPopup.h"
#include "Crafting/CraftingComponent.h"
#include "Crafting/RecipeData.h"
#include "Crafting/W_ScreenMessage.h"
#include "Gamemode/LobbyGameModeBase.h"
#include "Components/WrapBox.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/OverlaySlot.h"

void UW_CraftingWindow::NativeConstruct()
{
    Super::NativeConstruct();

    ALobbyGameModeBase* GM = Cast<ALobbyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GM)
    {
        CraftingComp = GM->CraftingComp;
    }

    if (CraftingComp)
    {
        CraftingComp->OnItemBagChanged.AddDynamic(this, &UW_CraftingWindow::RefreshItemPanel);
        CraftingComp->OnStagedIngredientsChanged.AddDynamic(this, &UW_CraftingWindow::RefreshItemPanel);
        CraftingComp->OnStagedIngredientsChanged.AddDynamic(this, &UW_CraftingWindow::RefreshStagingPanel);
        CraftingComp->OnCraftingFinished.AddDynamic(this, &UW_CraftingWindow::HandleCraftingFinished);
        CraftingComp->OnRecipeModeChanged.AddDynamic(this, &UW_CraftingWindow::HandleRecipeModeChanged);

        RefreshItemPanel();
        RefreshStagingPanel();
    }

    if (SelectRecipeButton)
    {
        SelectRecipeButton->OnClicked.AddDynamic(this, &UW_CraftingWindow::OnSelectRecipeButtonClicked);
    }
    if (CraftButton)
    {
        CraftButton->OnClicked.AddDynamic(this, &UW_CraftingWindow::OnCraftButtonClicked);
    }
    if (ClearButton)
    {
        ClearButton->OnClicked.AddDynamic(this, &UW_CraftingWindow::OnClearButtonClicked);
    }
    if (CloseBtn)
    {
        CloseBtn->OnClicked.AddDynamic(this, &UW_CraftingWindow::OnCloseBtnClicked);
    }
}

void UW_CraftingWindow::NativeDestruct()
{
    if (CraftingComp)
    {
        CraftingComp->OnItemBagChanged.RemoveDynamic(this, &UW_CraftingWindow::RefreshItemPanel);
        CraftingComp->OnStagedIngredientsChanged.RemoveDynamic(this, &UW_CraftingWindow::RefreshItemPanel);
        CraftingComp->OnStagedIngredientsChanged.RemoveDynamic(this, &UW_CraftingWindow::RefreshStagingPanel);
        CraftingComp->OnCraftingFinished.RemoveDynamic(this, &UW_CraftingWindow::HandleCraftingFinished);
        CraftingComp->OnRecipeModeChanged.RemoveDynamic(this, &UW_CraftingWindow::HandleRecipeModeChanged);
    }

    Super::NativeDestruct();
}

void UW_CraftingWindow::RefreshItemPanel()
{
    if (!ItemSlotGrid || !SlotWidgetClass || !CraftingComp) return;

    ItemSlotGrid->ClearChildren();

    const TArray<FItemQuantity>& Staged = CraftingComp->GetStagedIngredients();

    int32 Index = 0;
    for (const FItemQuantity& Entry : CraftingComp->GetItemBag())
    {
        int32 StagedAmount = 0;
        for (const FItemQuantity& S : Staged)
        {
            if (S.ItemID == Entry.ItemID) { StagedAmount = S.Amount; break; }
        }
        const int32 DisplayAmount = FMath::Max(0, Entry.Amount - StagedAmount);

        UW_CraftingSlot* SlotWidget = CreateWidget<UW_CraftingSlot>(this, SlotWidgetClass);
        if (!SlotWidget) continue;

        SlotWidget->Init(Entry.ItemID, DisplayAmount, ItemDataTable);
        SlotWidget->OnSlotClicked.AddDynamic(this, &UW_CraftingWindow::OnItemSlotClicked);
        UGridSlot* GridSlot = ItemSlotGrid->AddChildToGrid(SlotWidget, Index / NumColumns, Index % NumColumns);
        if (GridSlot) GridSlot->SetPadding(FMargin(8.f));
        ++Index;
    }
}

void UW_CraftingWindow::RefreshStagingPanel()
{
    if (!StagingPanel || !SlotWidgetClass || !CraftingComp) return;

    StagingPanel->ClearChildren();

    for (const FItemQuantity& Entry : CraftingComp->GetStagedIngredients())
    {
        UW_CraftingSlot* SlotWidget = CreateWidget<UW_CraftingSlot>(this, SlotWidgetClass);
        if (!SlotWidget) continue;

        SlotWidget->Init(Entry.ItemID, Entry.Amount, ItemDataTable);
        SlotWidget->OnSlotClicked.AddDynamic(this, &UW_CraftingWindow::OnStagingSlotClicked);        
        StagingPanel->AddChild(SlotWidget);
        SlotWidget->SetPadding(FMargin(8.f));
    }
}

void UW_CraftingWindow::OnItemSlotClicked(FName ItemID)
{
    if (CraftingComp) CraftingComp->StageIngredient(ItemID, 1);
}

void UW_CraftingWindow::OnStagingSlotClicked(FName ItemID)
{
    if (CraftingComp) CraftingComp->UnstageIngredient(ItemID, 1);
}

void UW_CraftingWindow::HandleCraftingFinished(bool bSuccess)
{
    UE_LOG(LogTemp, Log, TEXT("[Crafting] %s"), bSuccess ? TEXT("합성 성공") : TEXT("합성 실패"));

    if (bSuccess)
    {
        PrintMessageScreen("Success");
    }
    else
    {
        PrintMessageScreen("Failed");
    }
}

void UW_CraftingWindow::OnCraftButtonClicked()
{
    if (CraftingComp) CraftingComp->RequestFreeCraft();
}

void UW_CraftingWindow::OnClearButtonClicked()
{
    if (CraftingComp)
    {
        CraftingComp->ClearRecipeMode();
        CraftingComp->ClearStaged();
    }
}

void UW_CraftingWindow::OnCloseBtnClicked()
{
    RemoveFromParent();
}

void UW_CraftingWindow::OnSelectRecipeButtonClicked()
{
    if (!RecipePopupClass || !SubWindowOverlay || !CraftingComp) return;

    SubWindowOverlay->ClearChildren();

    UW_RecipeSelectPopup* Popup = CreateWidget<UW_RecipeSelectPopup>(this, RecipePopupClass);
    if (Popup)
    {
        Popup->SetCraftingComp(CraftingComp);
        SubWindowOverlay->AddChild(Popup);
    }
}

void UW_CraftingWindow::PrintMessageScreen(const FString& Message)
{
    if (!ScreenMessage || !SubWindowOverlay) return;

    SubWindowOverlay->ClearChildren();
    UW_ScreenMessage* Popup = CreateWidget<UW_ScreenMessage>(this, ScreenMessage);
    if (Popup)
    {
        Popup->InitScreenMessage(Message);
        UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(SubWindowOverlay->AddChild(Popup));

        //SubWindowOverlay->AddChild(Popup);
        if (OverlaySlot)
        {
            OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
            OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
        }
    }
}


void UW_CraftingWindow::HandleRecipeModeChanged(bool bIsRecipeMode, URecipeData* Recipe)
{
    if (!SelectedRecipeName) return;

    if (bIsRecipeMode && Recipe)
    {
        SelectedRecipeName->SetText(FText::FromString(Recipe->GetName()));
    }
    else
    {
        SelectedRecipeName->SetText(FText::FromString(TEXT("자유조합")));
    }
}
