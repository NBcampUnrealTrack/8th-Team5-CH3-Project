// TargetLockComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"

class ABaseEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLockTargetChanged, ABaseEnemy*, NewTarget);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLAMOR_LIBERTATIS_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "LockOn")
	FOnLockTargetChanged OnLockTargetChanged;

	UFUNCTION(BlueprintCallable, Category = "LockOn")
	ABaseEnemy* GetCurrentTarget() const;

	// Sets default values for this component's properties
	UTargetLockComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* OwnerCharacter;
	UPROPERTY()
	ABaseEnemy* CurrentTarget;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	float TraceRadius;
	UPROPERTY(EditAnywhere, Category = "LockOn")
	float MaxLockDistsq;
	UPROPERTY(EditAnywhere, Category = "LockOn")
	TEnumAsByte<ECollisionChannel> TargetChannel;

	AActor* FindTarget();
	void UpdateRotation(float DeltaTime);
	bool IsValidTarget(AActor* Target) const;
	bool ValidateCurrentTarget() const;

	void StartLock();
	void EndLock();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleLock();
};