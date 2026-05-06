#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Room UMETA(DisplayName = "Room"),
	Combat UMETA(DisplayName = "Combat"),
	Victory UMETA(DisplayName = "Victory"),
	Death UMETA(DisplayName = "Death")
};

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
	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category ="UI")
	TSubclassOf<UUserWidget>HUDWidgetClass;
	UPROPERTY()
	UUserWidget* HUDWidgetRef;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	EGameState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;
	UPROPERTY()
	UUserWidget* DeathWidgetRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> VictoryWidgetClass;
	UPROPERTY()
	UUserWidget* VictoryWidgetRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY()
	UUserWidget* MainMenuWidgetRef;


	UFUNCTION(BlueprintCallable)
	void SetGameState(EGameState NewState);
	UFUNCTION()
	void ShowDeathUI();
	UFUNCTION()
	void ShowVictoryUI();
	UFUNCTION()
	void ShowMainMenu();
	UFUNCTION()
	void HideMainMenu();
};
