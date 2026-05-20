#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_CraftingWindow.generated.h"

class UCraftingComponent;
class UW_CraftingSlot;
class UW_RecipeSelectPopup;
class UWrapBox;
class UGridPanel;
class UOverlay;
class UButton;
class UTextBlock;
class URecipeData;
class UW_ScreenMessage;

UCLASS()
class UW_CraftingWindow : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    TSubclassOf<UW_CraftingSlot> SlotWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    TSubclassOf<UW_RecipeSelectPopup> RecipePopupClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    TSubclassOf<UW_ScreenMessage> ScreenMessage;

    // 아이템 이름 조회용 (DT_ItemTable 연결)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    UDataTable* ItemDataTable;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // 가로 열 수 (에디터에서 조정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    int32 NumColumns = 4;

    // 오른쪽 아이템
    UPROPERTY(meta = (BindWidget))
    UGridPanel* ItemSlotGrid;

    // 왼쪽 - Staging
    UPROPERTY(meta = (BindWidget))
    UWrapBox* StagingPanel;

    UPROPERTY(meta = (BindWidget))
    UButton* CraftButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ClearButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseBtn;

    UPROPERTY(meta = (BindWidget))
    UButton* SelectRecipeButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SelectedRecipeName;

    // 팝업이 띄워지는 오버레이
    UPROPERTY(meta = (BindWidget))
    UOverlay* SubWindowOverlay;


private:
    UPROPERTY()
    UCraftingComponent* CraftingComp;

    UFUNCTION()
    void RefreshItemPanel();

    UFUNCTION()
    void RefreshStagingPanel();

    UFUNCTION()
    void OnItemSlotClicked(FName ItemID);

    UFUNCTION()
    void OnStagingSlotClicked(FName ItemID);

    UFUNCTION()
    void HandleCraftingFinished(bool bSuccess);

    UFUNCTION()
    void OnCraftButtonClicked();

    UFUNCTION()
    void OnClearButtonClicked();

    UFUNCTION()
    void OnCloseBtnClicked();

    UFUNCTION()
    void OnSelectRecipeButtonClicked();

    void PrintMessageScreen(const FString& Message);

    UFUNCTION()
    void HandleRecipeModeChanged(bool bIsRecipeMode, URecipeData* Recipe);
};
