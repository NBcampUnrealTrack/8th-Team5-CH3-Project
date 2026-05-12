// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RecipeData.generated.h"


USTRUCT(BlueprintType)
struct FItemQuantity
{
	GENERATED_BODY()

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// UItemData* ItemAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};



/**
 * 
 */
UCLASS(BlueprintType)
class URecipeData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 필요 재료 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TArray<FItemQuantity> Ingredients;

	// 결과물 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	FItemQuantity ResultItem;
};
