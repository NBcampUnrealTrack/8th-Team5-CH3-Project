// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gamemode/Scenario/ScenarioData.h"
#include "LobbyGameModeBase.generated.h"

class UScenarioManagerComponent;
class UCraftingComponent;
class UQuestionWidget;
class UQuestionAnswerWidget;
class ULobbyWidget;

UCLASS()
class ALobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	ALobbyGameModeBase();

	UFUNCTION(BlueprintCallable, Category="GameLogic")
	void ReadyComplete();

	UFUNCTION(BlueprintCallable, Category="GameLogic")
	void OpenCraftingWindow();

	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void GotoBattle();

	void ShowLobbyPhase();
	void ShowQuestionPhase(const TArray<FScenarioData>& Choices);

	UPROPERTY(EditAnywhere, Category = "Crafting")
	TSubclassOf<UUserWidget> CraftingWindowClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Question")
	TSubclassOf<UQuestionWidget> QuestionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Question")
	TSubclassOf<UQuestionAnswerWidget> AnswerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Lobby")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;


private:

	UPROPERTY()
	TObjectPtr<UQuestionWidget> QuestionWidget;
	UPROPERTY()
	TObjectPtr<UQuestionAnswerWidget> AnswerWidget;
	UPROPERTY()
	TObjectPtr<ULobbyWidget> LobbyWidget;
	UPROPERTY()
	TObjectPtr<UScenarioManagerComponent> ScenarioManagerComp;


	UFUNCTION()
	void OnQuestionSelected(FName NextID);
	UFUNCTION()
	void OnAnswerFinished();
	UFUNCTION()
	void HandleScenarioStepUpdated(const FScenarioData& MainData, const TArray<FScenarioData>& Choices);
	UFUNCTION()
	void HandleScenarioEnded();
	UFUNCTION()
	void OnStartBattleClicked();

	void ShowAnswerPhase(const FText& AnswerText);
	void HideAllWidgets();
	void SetMouseUI();
	void SetMouseGame();
};
