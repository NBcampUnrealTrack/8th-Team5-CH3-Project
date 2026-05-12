// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CraftingComponent.h"
#include "Crafting/RecipeData.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


bool UCraftingComponent::CanCraft(URecipeData* Recipe) const
{
	if (!Recipe) return false;

	// TODO:: 인벤토리 가져오기
	// AActor* Owner = GetOwner();
	// UInventoryComponent* Inventory = Owner->FindComponentByClass<UInventoryComponent>();

	// TODO:: 인벤토리에 필요 Item 조회
	for (const FItemQuantity& Ingredient : Recipe->Ingredients)
	{
		// TODO:: 개수가 충분한지 체크
		// Inventory->GetItemCount(Ingredient.ItemAsset) < Ingredient.Amount
		// 라면 return false;
	}

	return true;
}

void UCraftingComponent::RequestCraft(URecipeData* Recipe)
{
	if (CanCraft(Recipe))
	{
		ExecuteCraft(Recipe);
		OnCraftingFinished.Broadcast(true);
	}
	else
	{
		OnCraftingFinished.Broadcast(false);
	}
}

void UCraftingComponent::ExecuteCraft(URecipeData* Recipe)
{
	// 실제 아이템 소모 및 지급 로직 구현부
	// 재료 소모: Inventory->RemoveItem(Ingredient.ItemAsset, Ingredient.Amount);
	// 결과물 지급: Inventory->AddItem(Recipe->ResultItem.ItemAsset, Recipe->ResultItem.Amount);

	//_LOG(LogTemp, Log, TEXT("Crafting Successful: %s"), *Recipe->ResultItem.ItemAsset->GetName());
}