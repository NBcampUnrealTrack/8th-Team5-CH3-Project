#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_RecipeSelectPopup.generated.h"

class UCraftingComponent;
class UW_RecipeSlot;
class UScrollBox;
class UButton;
class URecipeData;

UCLASS()
class UW_RecipeSelectPopup : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    TSubclassOf<UW_RecipeSlot> RecipeSlotClass;

    // 외부에서 CraftingComp를 주입 (CraftingWindow에서 생성 시 호출)
    void SetCraftingComp(UCraftingComponent* InComp);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* RecipeScrollBox;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseBtn;

private:
    UPROPERTY()
    UCraftingComponent* CraftingComp;

    void RefreshRecipeList();

    UFUNCTION()
    void OnRecipeUseClicked(URecipeData* Recipe);

    UFUNCTION()
    void OnCloseBtnClicked();
};
