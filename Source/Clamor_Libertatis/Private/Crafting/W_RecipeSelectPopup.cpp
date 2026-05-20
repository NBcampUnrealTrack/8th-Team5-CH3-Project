#include "Crafting/W_RecipeSelectPopup.h"
#include "Crafting/W_RecipeSlot.h"
#include "Crafting/CraftingComponent.h"
#include "Crafting/FCraftingRecipeRow.h"
#include "Crafting/RecipeData.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"

void UW_RecipeSelectPopup::SetCraftingComp(UCraftingComponent* InComp)
{
    CraftingComp = InComp;
}

void UW_RecipeSelectPopup::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseBtn)
    {
        CloseBtn->OnClicked.AddDynamic(this, &UW_RecipeSelectPopup::OnCloseBtnClicked);
    }

    RefreshRecipeList();
}

void UW_RecipeSelectPopup::RefreshRecipeList()
{
    if (!RecipeScrollBox || !RecipeSlotClass || !CraftingComp) return;
    if (!CraftingComp->RecipeDataTable) return;

    RecipeScrollBox->ClearChildren();

    TArray<FCraftingRecipeRow*> Rows;
    CraftingComp->RecipeDataTable->GetAllRows<FCraftingRecipeRow>(TEXT("RecipeSelectPopup"), Rows);

    for (FCraftingRecipeRow* Row : Rows)
    {
        if (!Row || !Row->RecipeAsset) continue;

        UW_RecipeSlot* SlotWidget = CreateWidget<UW_RecipeSlot>(this, RecipeSlotClass);
        if (!SlotWidget) continue;

        SlotWidget->Init(Row->RecipeAsset, Row->RecipeDisplayName, CraftingComp);
        SlotWidget->OnRecipeUseClicked.AddDynamic(this, &UW_RecipeSelectPopup::OnRecipeUseClicked);
        RecipeScrollBox->AddChild(SlotWidget);
    }
}

void UW_RecipeSelectPopup::OnRecipeUseClicked(URecipeData* Recipe)
{
    if (CraftingComp)
    {
        CraftingComp->ApplyRecipe(Recipe);
    }
    RemoveFromParent();
}

void UW_RecipeSelectPopup::OnCloseBtnClicked()
{
    RemoveFromParent();
}
