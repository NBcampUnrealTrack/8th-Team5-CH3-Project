// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RecipeData.h"
#include "FCraftingRecipeRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCraftingRecipeRow : public FTableRowBase
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    FString RecipeDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    class URecipeData* RecipeAsset;
};
