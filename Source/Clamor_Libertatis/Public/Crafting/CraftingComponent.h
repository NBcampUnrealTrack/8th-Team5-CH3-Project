#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crafting/RecipeData.h"
#include "CraftingComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingFinished, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStagedIngredientsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemBagChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRecipeModeChanged, bool, bIsRecipeMode, URecipeData*, Recipe);

class URecipeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UCraftingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCraftingComponent();

    UPROPERTY(BlueprintAssignable, Category = "Crafting")
    FOnCraftingFinished OnCraftingFinished;

    UPROPERTY(BlueprintAssignable, Category = "Crafting")
    FOnStagedIngredientsChanged OnStagedIngredientsChanged;

    UPROPERTY(BlueprintAssignable, Category = "Crafting")
    FOnItemBagChanged OnItemBagChanged;

    UPROPERTY(BlueprintAssignable, Category = "Crafting")
    FOnRecipeModeChanged OnRecipeModeChanged;

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void AddItem(FName ItemID, int32 Amount = 1);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void RemoveItem(FName ItemID, int32 Amount = 1);

    UFUNCTION(BlueprintPure, Category = "Crafting")
    int32 GetItemAmount(FName ItemID) const;

    UFUNCTION(BlueprintPure, Category = "Crafting")
    const TArray<FItemQuantity>& GetItemBag() const { return ItemBag; }

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void SetItemBag(const TArray<FItemQuantity>& InItems);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void RequestCraft(URecipeData* Recipe);

    UFUNCTION(BlueprintPure, Category = "Crafting")
    bool CanCraft(URecipeData* Recipe) const;

    // ── 자유 투입 방식 ───────────────────────────
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool StageIngredient(FName ItemID, int32 Amount = 1);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void UnstageIngredient(FName ItemID, int32 Amount = 1);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void ClearStaged();

    // 레시피 일치 → 성공, 불일치 → 재료 소멸 후 실패
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void RequestFreeCraft();

    // ── 레시피 모드 ─────────────────────────────
    // 레시피 선택 → 재료 자동 스테이징 + 모드 진입
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void ApplyRecipe(URecipeData* Recipe);

    // 레시피 모드 해제 (자유조합으로 전환)
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void ClearRecipeMode();

    UFUNCTION(BlueprintPure, Category = "Crafting")
    bool IsRecipeMode() const { return ActiveRecipe != nullptr; }

    UFUNCTION(BlueprintPure, Category = "Crafting")
    URecipeData* GetActiveRecipe() const { return ActiveRecipe; }

    UFUNCTION(BlueprintPure, Category = "Crafting")
    const TArray<FItemQuantity>& GetStagedIngredients() const { return StagedIngredients; }


    // 레시피 테이블 (에디터에서 DT_CraftingRecipeRow 연결)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    UDataTable* RecipeDataTable;

    // 아이템 이름/아이콘 조회용 테이블
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    UDataTable* ItemDataTable;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Crafting")
    TArray<FItemQuantity> ItemBag;

    TArray<FItemQuantity> StagedIngredients;

    UPROPERTY()
    URecipeData* ActiveRecipe = nullptr;

    void ExecuteCraft(URecipeData* Recipe);
    URecipeData* FindMatchingRecipe() const;
    bool BagHasStaged() const;
    int32 GetStagedAmount(FName ItemID) const;
};
