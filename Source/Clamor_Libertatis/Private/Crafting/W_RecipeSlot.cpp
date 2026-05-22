#include "Crafting/W_RecipeSlot.h"
#include "Crafting/CraftingComponent.h"
#include "Crafting/RecipeData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

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

    if (RecipeNameText && InRecipe)
    {
        RecipeNameText->SetText(InRecipe->ResultItem.DisplayName);
    }

    if (IngredientsText && InRecipe)
    {
        FString IngredientStr;
        for (const FItemQuantity& Ing : InRecipe->Ingredients)
        {
            IngredientStr += FString::Printf(TEXT("%s x%d  "), *Ing.DisplayName.ToString(), Ing.Amount);
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
