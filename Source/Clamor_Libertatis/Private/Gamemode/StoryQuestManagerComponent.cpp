#include "Gamemode/StoryQuestManagerComponent.h"
#include "Gamemode/CLGameInstance.h"

UStoryQuestManagerComponent::UStoryQuestManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<FName> UStoryQuestManagerComponent::GetAvailableQuestions(int32 Phase) const
{
	TArray<FName> Result;
	if (!QuestionDataTable) return Result;

	TArray<FName> RowNames = QuestionDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FStoryQA* Row = QuestionDataTable->FindRow<FStoryQA>(RowName, TEXT(""));
		if (!Row || Row->Phase != Phase) continue;
		if (IsViewed(RowName)) continue;

		// 선행질문 조건 확인
		if (Row->PrerequisiteQuestion != NAME_None && !IsViewed(Row->PrerequisiteQuestion)) continue;

		Result.Add(RowName);
	}
	return Result;
}

bool UStoryQuestManagerComponent::GetQuestion(FName RowName, FStoryQA& OutQuestion) const
{
	if (!QuestionDataTable) return false;

	const FStoryQA* Row = QuestionDataTable->FindRow<FStoryQA>(RowName, TEXT(""));
	if (!Row) return false;

	OutQuestion = *Row;
	return true;
}

void UStoryQuestManagerComponent::MarkViewed(FName RowName)
{
	if (UCLGameInstance* GI = GetGameInst())
	{
		GI->AddViewedQuestion(RowName);
	}
}

bool UStoryQuestManagerComponent::IsViewed(FName RowName) const
{
	if (const UCLGameInstance* GI = GetGameInst())
	{
		return GI->HasViewedQuestion(RowName);
	}
	return false;
}

UCLGameInstance* UStoryQuestManagerComponent::GetGameInst() const
{
	if (const UWorld* World = GetWorld())
	{
		return World->GetGameInstance<UCLGameInstance>();
	}
	return nullptr;
}
