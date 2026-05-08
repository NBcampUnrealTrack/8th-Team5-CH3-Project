#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Gamemode/StageGameModeBase.h" 
#include "BasePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UPlayerHUDWidget;
class UEnemyHPBarWidget;
class ABaseEnemy;
class UUserWidget;

UENUM(BlueprintType)
enum class EUIType : uint8
{
	GameStart,
	HUD,
	Lobby,
	Death,
	Victory,
	MainMenu,
	EnemyHPBar
};

USTRUCT(BlueprintType)
struct FWidgetEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ZOrder = 0;
};

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* BasicAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* ActiveSkillAction;

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
	UFUNCTION(BlueprintCallable, Category = "UI")
	FORCEINLINE UPlayerHUDWidget* GetHUDWidget() const{return HUDWidgetRef;}

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> GameStartWidgetClass;
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
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEnemyHPBarWidget> EnemyHPBarWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	float EnemyHPBarShowDistance = 700.f;


	UPROPERTY()
	TObjectPtr<UEnemyHPBarWidget> EnemyHPBarWidget;
	UPROPERTY()
	TObjectPtr<UPlayerHUDWidget> HUDWidgetRef;

	UPROPERTY(EditAnywhere, Category = "UI")
	float HPBarUpdateInterval = 0.15f;

	FTimerHandle HPBarUpdateTimerHandle;

	void UpdateEnemyHPBar();

private:
	static constexpr int32 ZOrder_HUD = 0;
	static constexpr int32 ZOrder_Victory = 10;
	static constexpr int32 ZOrder_Death = 20;
	static constexpr int32 ZOrder_MainMenu = 30;

	UPROPERTY() 
	TObjectPtr<UUserWidget> GameStartWidgetRef;
	UPROPERTY()
	TObjectPtr<UUserWidget> LobbyWidgetRef;
	UPROPERTY()
	TObjectPtr<UUserWidget> DeathWidgetRef;
	UPROPERTY()
	TObjectPtr<UUserWidget> VictoryWidgetRef;
	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenuWidgetRef;

	template<typename T>
	T* GetOrCreateWidget(TObjectPtr<T>& WidgetRef, TSubclassOf<T> WidgetClass, int32 ZOrder = 0)
	{
		if (!WidgetRef && WidgetClass)
		{
			WidgetRef = CreateWidget<T>(this, WidgetClass);
			if (WidgetRef) WidgetRef->AddToViewport(ZOrder);
		}
		return WidgetRef.Get();
	}

	void ShowWidgetInternal(TObjectPtr<UUserWidget>& WidgetRef, TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);
	void HideWidgetInternal(TObjectPtr<UUserWidget>& WidgetRef);

	void InitHUDWidget();
	ABaseEnemy* FindClosestEnemy() const;
};
