#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ItemTableRow.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon      UMETA(DisplayName = "무기"),
    Armor       UMETA(DisplayName = "방어구"),
    Consumable  UMETA(DisplayName = "소비 아이템"),
    Quest       UMETA(DisplayName = "퀘스트 아이템"),
    SocketItem  UMETA(DisplayName = "무기 소켓 아이템")
};

UENUM(BlueprintType)
enum class EConsumableEffectType : uint8
{
    None        UMETA(DisplayName = "없음"),
    Heal        UMETA(DisplayName = "체력 회복"),
    Mana        UMETA(DisplayName = "마나 회복"),
    Stamina     UMETA(DisplayName = "스태미너 회복"),
    Bomb        UMETA(DisplayName = "폭탄")
};

class UWeaponSocketItemData;

USTRUCT(BlueprintType)
struct CLAMOR_LIBERTATIS_API FItemTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemType ItemType = EItemType::Consumable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    EConsumableEffectType EffectType = EConsumableEffectType::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    float EffectValue = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    float EffectRadius = 0.f;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    float EffectDuration = 0.f;   

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SocketItem")
    TSoftObjectPtr<UWeaponSocketItemData> SocketItemData;
};