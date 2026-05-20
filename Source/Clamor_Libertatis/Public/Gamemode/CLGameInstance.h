// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item/Inventory/InventoryComponent.h"
#include "CLGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCLGameInstance();

	void AddWonBattle();
	int32 GetWonBattleCount() const { return WonBattleCount; }
	
	const TSet<FName>& GetAllViewedQuestions() const { return ViewedQuestions; }

	bool HasWatchedOpening() const;
	

	void RegisterViewedQuestion(const FString& PrefixKey);
	bool HasViewedQuestion(FName RowName) const { return ViewedQuestions.Contains(RowName); }

	void SaveInventory(const TArray<FInventorySlot>& Slots);
	const TArray<FInventorySlot>& GetSavedInventory() const { return SavedInventorySlots; }

	FName LastScenarioRowName;

private:
	int32 WonBattleCount;

	UPROPERTY()
	TSet<FName> ViewedQuestions;

	TArray<FInventorySlot> SavedInventorySlots;
};
