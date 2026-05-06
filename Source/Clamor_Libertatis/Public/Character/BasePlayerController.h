#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Gamemode/StageGameModeBase.h" 
#include "BasePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CLAMOR_LIBERTATIS_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	virtual void BeginPlay() override;

	// IA, IMC ¿¬°á
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



	//UI

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

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameStartWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> GameStartWidgetRef;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;  
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> HUDWidgetRef;

	UPROPERTY()
	TObjectPtr<UUserWidget> LobbyWidgetRef;

	UPROPERTY()
	TObjectPtr<UUserWidget> DeathWidgetRef;

	UPROPERTY()
	TObjectPtr<UUserWidget> VictoryWidgetRef;

	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenuWidgetRef;

	
};
