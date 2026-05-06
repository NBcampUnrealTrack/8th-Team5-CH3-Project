// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LoreManagerComponent.generated.h"


UCLASS()
class ULoreManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULoreManagerComponent();

	UFUNCTION(BlueprintCallable, Category = "Lore")
	FText GetRandomLoreText(int32 Phase);

protected:
	UPROPERTY(EditAnywhere, Category = "Lore Settings")
	FString KeyPrefix;

	UPROPERTY(EditAnywhere, Category = "Lore Settings")
	TObjectPtr<UStringTable> ST;
		
};
