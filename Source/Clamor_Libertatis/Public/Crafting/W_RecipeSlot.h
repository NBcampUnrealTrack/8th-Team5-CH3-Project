#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_RecipeSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeUseClicked, URecipeData*, Recipe);

class UButton;
class UTextBlock;
class UCraftingComponent;
class URecipeData;

UCLASS()
class UW_RecipeSlot : public UUserWidget
{
    GENERATED_BODY()

public:
    void Init(URecipeData* InRecipe, const FString& DisplayName, UCraftingComponent* InComp);

    UPROPERTY(BlueprintAssignable, Category = "Crafting")
    FOnRecipeUseClicked OnRecipeUseClicked;

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* RecipeNameText;

    // 필요 재료 목록 (선택)
    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* IngredientsText;

    UPROPERTY(meta = (BindWidget))
    UButton* UseButton;

private:
    UPROPERTY()
    URecipeData* CachedRecipe;

    UFUNCTION()
    void HandleUseClicked();
};
