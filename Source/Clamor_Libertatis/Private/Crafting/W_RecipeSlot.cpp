#include "Crafting/W_RecipeSlot.h"
#include "Crafting/CraftingComponent.h"
#include "Crafting/RecipeData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Item/ItemTableRow.h"

void UW_RecipeSlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (UseButton)
    {
        UseButton->OnClicked.AddDynamic(this, &UW_RecipeSlot::HandleUseClicked);
    }
}

void UW_RecipeSlot::Init(URecipeData* InRecipe, UCraftingComponent* InComp)
{
    CachedRecipe = InRecipe;
    if (!InRecipe) return;

    UDataTable* ItemTable = InComp ? InComp->ItemDataTable : nullptr;

    auto GetItemName = [ItemTable](FName ItemID) -> FString
    {
        if (ItemTable)
        {
            const FItemTableRow* Row = ItemTable->FindRow<FItemTableRow>(ItemID, TEXT(""));
            if (Row) return Row->ItemName.ToString();
        }
        return ItemID.ToString();
    };

    if (RecipeNameText)
    {
        RecipeNameText->SetText(FText::FromString(GetItemName(InRecipe->ResultItem.ItemID)));
    }

    if (IngredientsText)
    {
        FString IngredientStr;
        for (const FItemQuantity& Ing : InRecipe->Ingredients)
        {
            IngredientStr += FString::Printf(TEXT("%s x%d  "), *GetItemName(Ing.ItemID), Ing.Amount);
        }
        IngredientsText->SetText(FText::FromString(IngredientStr.TrimEnd()));
    }

    if (UseButton && InComp)
    {
        UseButton->SetIsEnabled(InComp->CanCraft(InRecipe));
    }
}

void UW_RecipeSlot::HandleUseClicked()
{
    OnRecipeUseClicked.Broadcast(CachedRecipe);
}
