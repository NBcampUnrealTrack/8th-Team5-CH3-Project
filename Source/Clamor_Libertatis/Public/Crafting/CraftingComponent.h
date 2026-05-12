// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CraftingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingFinished, bool, bSuccess);

//class UItemData;
class URecipeData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void RequestCraft(URecipeData* Recipe);

	UFUNCTION(BlueprintPure, Category = "Crafting")
	bool CanCraft(URecipeData* Recipe) const;
		
	UPROPERTY(BlueprintAssignable, Category = "Crafting")
	FOnCraftingFinished OnCraftingFinished;

protected:
	void ExecuteCraft(URecipeData* Recipe);
};
