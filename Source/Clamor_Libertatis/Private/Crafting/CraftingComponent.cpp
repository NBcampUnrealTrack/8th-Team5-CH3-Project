#include "Crafting/CraftingComponent.h"
#include "Crafting/RecipeData.h"
#include "Crafting/FCraftingRecipeRow.h"

UCraftingComponent::UCraftingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// ── 재료 풀 관리 ──────────────────────────────────────────────────────────────

void UCraftingComponent::AddItem(FName ItemID, int32 Amount)
{
    if (ItemID.IsNone() || Amount <= 0) return;

    for (FItemQuantity& Entry : ItemBag)
    {
        if (Entry.ItemID == ItemID)
        {
            Entry.Amount += Amount;
            OnItemBagChanged.Broadcast();
            return;
        }
    }

    // 새 아이템은 맨 앞에 추가
    FItemQuantity NewEntry;
    NewEntry.ItemID = ItemID;
    NewEntry.Amount = Amount;
    ItemBag.Insert(NewEntry, 0);
    OnItemBagChanged.Broadcast();
}

void UCraftingComponent::RemoveItem(FName ItemID, int32 Amount)
{
    for (int32 i = 0; i < ItemBag.Num(); ++i)
    {
        if (ItemBag[i].ItemID == ItemID)
        {
            ItemBag[i].Amount -= Amount;
            if (ItemBag[i].Amount <= 0)
            {
                ItemBag.RemoveAt(i);
            }
            OnItemBagChanged.Broadcast();
            return;
        }
    }
}

int32 UCraftingComponent::GetItemAmount(FName ItemID) const
{
    for (const FItemQuantity& Entry : ItemBag)
    {
        if (Entry.ItemID == ItemID) return Entry.Amount;
    }
    return 0;
}

// ── 레시피 지정 방식 ──────────────────────────────────────────────────────────

bool UCraftingComponent::CanCraft(URecipeData* Recipe) const
{
    if (!Recipe) return false;

    for (const FItemQuantity& Ingredient : Recipe->Ingredients)
    {
        if (GetItemAmount(Ingredient.ItemID) < Ingredient.Amount)
        {
            return false;
        }
    }
    return true;
}

void UCraftingComponent::RequestCraft(URecipeData* Recipe)
{
    if (CanCraft(Recipe))
    {
        ExecuteCraft(Recipe);
        OnCraftingFinished.Broadcast(true);
    }
    else
    {
        OnCraftingFinished.Broadcast(false);
    }
}

// ── 자유 투입 방식 ────────────────────────────────────────────────────────────

int32 UCraftingComponent::GetStagedAmount(FName ItemID) const
{
    for (const FItemQuantity& Entry : StagedIngredients)
    {
        if (Entry.ItemID == ItemID) return Entry.Amount;
    }
    return 0;
}

bool UCraftingComponent::StageIngredient(FName ItemID, int32 Amount)
{
    if (ActiveRecipe) ClearRecipeMode();

    int32 AlreadyStaged = GetStagedAmount(ItemID);
    if (GetItemAmount(ItemID) < AlreadyStaged + Amount)
    {
        return false;
    }

    for (FItemQuantity& Entry : StagedIngredients)
    {
        if (Entry.ItemID == ItemID)
        {
            Entry.Amount += Amount;
            OnStagedIngredientsChanged.Broadcast();
            return true;
        }
    }

    FItemQuantity NewEntry;
    NewEntry.ItemID = ItemID;
    NewEntry.Amount = Amount;
    StagedIngredients.Add(NewEntry);
    OnStagedIngredientsChanged.Broadcast();
    return true;
}

void UCraftingComponent::UnstageIngredient(FName ItemID, int32 Amount)
{
    if (ActiveRecipe) ClearRecipeMode();

    for (int32 i = 0; i < StagedIngredients.Num(); ++i)
    {
        if (StagedIngredients[i].ItemID == ItemID)
        {
            StagedIngredients[i].Amount -= Amount;
            if (StagedIngredients[i].Amount <= 0)
            {
                StagedIngredients.RemoveAt(i);
            }
            OnStagedIngredientsChanged.Broadcast();
            return;
        }
    }
}

void UCraftingComponent::ClearStaged()
{
    StagedIngredients.Empty();
    OnStagedIngredientsChanged.Broadcast();
}

URecipeData* UCraftingComponent::FindMatchingRecipe() const
{
    if (!RecipeDataTable || StagedIngredients.IsEmpty()) return nullptr;

    TArray<FCraftingRecipeRow*> Rows;
    RecipeDataTable->GetAllRows<FCraftingRecipeRow>(TEXT("FindMatchingRecipe"), Rows);

    URecipeData* BestMatch = nullptr;
    int32 BestTotal = -1;

    for (FCraftingRecipeRow* Row : Rows)
    {
        if (!Row || !Row->RecipeAsset) continue;

        const TArray<FItemQuantity>& Required = Row->RecipeAsset->Ingredients;
        if (Required.Num() != StagedIngredients.Num()) continue;

        bool bMatch = true;
        int32 TotalRequired = 0;
        for (const FItemQuantity& Req : Required)
        {
            bool bFound = false;
            for (const FItemQuantity& Staged : StagedIngredients)
            {
                if (Staged.ItemID == Req.ItemID && Staged.Amount >= Req.Amount)
                {
                    bFound = true;
                    TotalRequired += Req.Amount;
                    break;
                }
            }
            if (!bFound) { bMatch = false; break; }
        }

        if (bMatch && TotalRequired > BestTotal)
        {
            BestTotal = TotalRequired;
            BestMatch = Row->RecipeAsset;
        }
    }

    return BestMatch;
}

bool UCraftingComponent::BagHasStaged() const
{
    for (const FItemQuantity& Entry : StagedIngredients)
    {
        if (GetItemAmount(Entry.ItemID) < Entry.Amount)
        {
            return false;
        }
    }
    return true;
}

void UCraftingComponent::RequestFreeCraft()
{
    if (StagedIngredients.IsEmpty())
    {
        OnCraftingFinished.Broadcast(false);
        return;
    }

    if (!BagHasStaged())
    {
        ClearStaged();
        OnCraftingFinished.Broadcast(false);
        return;
    }

    URecipeData* Matched = FindMatchingRecipe();

    for (const FItemQuantity& Entry : StagedIngredients)
    {
        RemoveItem(Entry.ItemID, Entry.Amount);
    }
    ClearStaged();

    if (Matched)
    {
        AddItem(Matched->ResultItem.ItemID, Matched->ResultItem.Amount);
        OnCraftingFinished.Broadcast(true);
    }
    else
    {
        OnCraftingFinished.Broadcast(false);
    }
}

// ── 레시피 모드 ───────────────────────────────────────────────────────────────

void UCraftingComponent::ApplyRecipe(URecipeData* Recipe)
{
    if (!Recipe || !CanCraft(Recipe)) return;

    ClearStaged();
    // ActiveRecipe는 StageIngredient 호출 전에 null 상태여야 ClearRecipeMode가 안 불림
    for (const FItemQuantity& Ingredient : Recipe->Ingredients)
    {
        StageIngredient(Ingredient.ItemID, Ingredient.Amount);
    }
    ActiveRecipe = Recipe;
    OnRecipeModeChanged.Broadcast(true, Recipe);
}

void UCraftingComponent::ClearRecipeMode()
{
    if (!ActiveRecipe) return;
    ActiveRecipe = nullptr;
    OnRecipeModeChanged.Broadcast(false, nullptr);
}

// ── 공통 ──────────────────────────────────────────────────────────────────────

void UCraftingComponent::ExecuteCraft(URecipeData* Recipe)
{
    for (const FItemQuantity& Ingredient : Recipe->Ingredients)
    {
        RemoveItem(Ingredient.ItemID, Ingredient.Amount);
    }
    AddItem(Recipe->ResultItem.ItemID, Recipe->ResultItem.Amount);
}
