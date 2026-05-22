#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UTextBlock;
class UButton;
class UW_CraftingWindow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartBattleClicked);

UCLASS()
class CLAMOR_LIBERTATIS_API ULobbyWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void InitLobby(int32 WonBattleCount);

    UPROPERTY(BlueprintAssignable)
    FOnStartBattleClicked OnStartBattleClicked;

    //크래프팅 윈도우 연결 필요
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UW_CraftingWindow> CraftingWindowClass;

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_WonCount;    // 완료한 시련

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_NextBattle;  // 다음 시련

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_SubMessage;  // 하단 메시지

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Btn_StartBattle;     // 시련 시작하기

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    TObjectPtr<UWidgetAnimation> Anim_FadeIn;

    UPROPERTY(meta = (BindWidget))
    UButton* StatButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CraftingButton;


private:
    UFUNCTION()
    void OnStartClicked();

    UFUNCTION()
    void OnStatButtonClicked();

    UFUNCTION()
    void OnCraftingButtonClicked();
};