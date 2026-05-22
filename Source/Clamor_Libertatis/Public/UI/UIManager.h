#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "UIManager.generated.h"

class APlayerController;
class UUserWidget;
class USkillComponent;
class USkillCooldownWidget;

UENUM(BlueprintType)
enum class EUIType : uint8
{
	Opening, 
	GameStart,
	Lobby,
	Death,
	Victory,
	MainMenu,
	QuickSlot,
	MasterInventory
};

USTRUCT(BlueprintType)
struct FWidgetEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> WidgetInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ZOrder = 0;
};


UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class CLAMOR_LIBERTATIS_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* InOwner);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidget(EUIType Type);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideWidget(EUIType Type);

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetWidget(EUIType Type) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsWidgetVisible(EUIType Type) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TMap<EUIType, FWidgetEntry> WidgetEntries;
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetOrCreateWidget(EUIType Type);

private:
	UPROPERTY()
	TObjectPtr<APlayerController> OwnerController;

	UUserWidget* GetOrCreate(EUIType Type);
};