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

void UW_RecipeSlot::Init(URecipeData* InRecipe, const FString& DisplayName, UCraftingComponent* InComp)
{
    CachedRecipe = InRecipe;

    if (RecipeNameText)
    {
        RecipeNameText->SetText(FText::FromString(DisplayName));
    }

    // 재료 목록 텍스트 (있는 경우)
    if (IngredientsText && InRecipe)
    {
        FString IngredientStr;
        for (const FItemQuantity& Ing : InRecipe->Ingredients)
        {
            IngredientStr += FString::Printf(TEXT("%s x%d  "), *Ing.ItemID.ToString(), Ing.Amount);
        }
        IngredientsText->SetText(FText::FromString(IngredientStr.TrimEnd()));
    }

    // 재료 충분 여부로 버튼 활성화
    if (UseButton && InComp)
    {
        UseButton->SetIsEnabled(InComp->CanCraft(InRecipe));
    }
}

void UW_RecipeSlot::HandleUseClicked()
{
    OnRecipeUseClicked.Broadcast(CachedRecipe);
}
