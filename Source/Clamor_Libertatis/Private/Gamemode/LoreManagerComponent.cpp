// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/LoreManagerComponent.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"


// Sets default values for this component's properties
ULoreManagerComponent::ULoreManagerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    KeyPrefix = TEXT("Lobby_Lore_Phase");
}

FText ULoreManagerComponent::GetRandomLoreText(int32 Phase)
{
    FString SearchPattern = FString::Printf(TEXT("%s0_"), *KeyPrefix);    
    
    TArray<FString> ValidKeys;

    if (ST)
    {
        FName TableID = ST->GetStringTableId();

        FStringTablePtr TablePtr = FStringTableRegistry::Get().FindMutableStringTable(TableID);


        TablePtr->EnumerateSourceStrings([&](const FString& InKey, const FString& InSourceString) -> bool {
            if (InKey.StartsWith(SearchPattern)) {
                ValidKeys.Add(InKey);
            }
            return true;
            });
    }
    else
    {
        return FText::FromString("String Table is Not Valid.");
    }

    if (ValidKeys.Num() > 0)
    {
        int32 RandomIdx = FMath::RandRange(0, ValidKeys.Num() - 1);
        return FText::FromStringTable(ST->GetStringTableId(), ValidKeys[RandomIdx]);
    }


    return FText::FromString("Load Failed.");
}



