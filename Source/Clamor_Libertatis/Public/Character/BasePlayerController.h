#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Gamemode/StageGameModeBase.h" 
#include "BasePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UPlayerHUDWidget;
class UUIManager;
class UEnemyTrackerComponent;
class USkillComponent;
class UQuickSlotWidget;
class UInventoryWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* BasicAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ActiveSkillAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LockAction;

	//UI
	UPROPERTY(EditAnywhere, Instanced, Category = "UI")
	TObjectPtr<UUIManager> UIManager;

	UPROPERTY(EditAnywhere, Instanced, Category = "UI")
	TObjectPtr<UEnemyTrackerComponent> EnemyTracker;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> HUDWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetStageState(ECheckStageResult NewState);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowGameStartUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideGameStartUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowLobbyUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideLobbyUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowDeathUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideDeathUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowVictoryUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideVictoryUI();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowMainMenu();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ContinueGame();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RestartGame();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void QuitGame();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowInventory();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideInventory();

	UFUNCTION(BlueprintCallable, Category = "UI")
	FORCEINLINE UPlayerHUDWidget* GetHUDWidget() const { return HUDWidgetRef; }

private:
	static constexpr int32 ZOrder_HUD = 0;
	static constexpr int32 ZOrder_Victory = 10;
	static constexpr int32 ZOrder_Death = 20;
	static constexpr int32 ZOrder_MainMenu = 30;

	UPROPERTY()
	TObjectPtr<UPlayerHUDWidget> HUDWidgetRef;
	UPROPERTY()
	TObjectPtr<UQuickSlotWidget> QuickSlotWidgetRef;

	void InitializeInput();
	void InitHUDWidget();
	void InitQuickSlotWidget();

	void UseQuickSlot1();
	void UseQuickSlot2();
	void UseQuickSlot3();
	void UseQuickSlot4();
};