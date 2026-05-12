#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gamemode/StoryQA.h"
#include "StoryQuestManagerComponent.generated.h"

class UCLGameInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UStoryQuestManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStoryQuestManagerComponent();

	// Phase 조건 + 선행질문 충족 + 미조회 항목만 반환
	UFUNCTION(BlueprintCallable, Category = "Story")
	TArray<FName> GetAvailableQuestions(int32 Phase) const;

	// Row Name으로 질문 데이터 조회
	UFUNCTION(BlueprintCallable, Category = "Story")
	bool GetQuestion(FName RowName, FStoryQA& OutQuestion) const;

	// 조회 완료 처리 (GameInstance에 저장)
	UFUNCTION(BlueprintCallable, Category = "Story")
	void MarkViewed(FName RowName);

	UFUNCTION(BlueprintPure, Category = "Story")
	bool IsViewed(FName RowName) const;

private:
	UCLGameInstance* GetGameInst() const;

	UPROPERTY(EditAnywhere, Category = "Story")
	TObjectPtr<UDataTable> QuestionDataTable;
};
