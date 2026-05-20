#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "DropItem.generated.h"

UCLASS()
class CLAMOR_LIBERTATIS_API ADropItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ADropItem();
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Mesh")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	
	UFUNCTION()
	void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	virtual void Tick(float DeltaTime) override;
};
