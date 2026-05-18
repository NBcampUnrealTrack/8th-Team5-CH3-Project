// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameModeBase.generated.h"

// TODO:: 더 이상 쓰지 않음 삭제 예정
// class ULoreManagerComponent;

class UScenarioManagerComponent;

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
	
	// 배틀로 진입함
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void GotoBattle();


	// Opening Widget
	UPROPERTY(EditAnywhere) 
	TSubclassOf<UUserWidget> OpeningWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scenario")
	class UScenarioManagerComponent* ScenarioManagerComp;


private:

	//TODO:: 기획상 더 이상 사용하지 않음.
	// 시나리오 관리 기능 도입 후 완전 삭제
	/*UPROPERTY(VisibleAnywhere, Category = "GameLogic")
	TObjectPtr<ULoreManagerComponent> LoreManagerComp;*/
	
};
