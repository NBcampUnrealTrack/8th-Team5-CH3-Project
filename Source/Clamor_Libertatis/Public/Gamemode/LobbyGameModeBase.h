// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameModeBase.generated.h"

class ULoreManagerComponent;

/**
 * 
 */
UCLASS()
class ALobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	ALobbyGameModeBase();

	// 로비에서 할 수 있는 메뉴들을 띄움
	void LobbyLoop();
	
	// 준비를 마침
	UFUNCTION(BlueprintCallable, Category="GameLogic")
	void ReadyComplete();	
	
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void LookAround();

	// 배틀로 진입함
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void GotoBattle();

private:
	UPROPERTY(VisibleAnywhere, Category = "GameLogic")
	TObjectPtr<ULoreManagerComponent> LoreManagerComp;
	
};
