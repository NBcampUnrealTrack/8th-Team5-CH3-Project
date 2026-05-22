// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTextActor.generated.h"

class UWidgetComponent;
class UDamageTextWidget;

UCLASS()
class CLAMOR_LIBERTATIS_API ADamageTextActor : public AActor
{
	GENERATED_BODY()
	
public:
	ADamageTextActor();

	virtual void BeginPlay() override;

	void InitDamage(float Damage);

protected:
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* WidgetComp;

	UPROPERTY()
	UDamageTextWidget* DamageWidget;
};