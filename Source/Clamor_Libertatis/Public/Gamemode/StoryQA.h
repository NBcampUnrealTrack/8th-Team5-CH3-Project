#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StoryQA.generated.h"

USTRUCT(BlueprintType)
struct FStoryQA : public FTableRowBase
{
	GENERATED_BODY()

	// 이 질문이 등장하는 Phase (0부터 시작)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	int32 Phase = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	FText Question;

	// 선택지 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	TArray<FText> Choices;

	// Choices와 1:1 대응하는 답변
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	TArray<FText> Answers;

	// 이 질문이 등장하려면 먼저 조회되어야 하는 질문의 Row Name
	// NAME_None이면 선행 조건 없음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	FName PrerequisiteQuestion = NAME_None;
};
