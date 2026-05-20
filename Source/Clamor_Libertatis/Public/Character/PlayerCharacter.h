//PlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item/Inventory/ConsumableInventoryComponent.h"
#include "Item/Inventory/SocketItemInventoryComponent.h"
#include "Item/InterFace/PickupItemInterface.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCombatComponent;
class UHealthComponent;
class USkillComponent;
class AWeaponBase;
struct FInputActionValue;
class UHealthComponent;
class UTargetLockComponent;
class UParticleSystem;
class USoundBase;


UENUM(BlueprintType)
enum class EDodgeDirection : uint8
{
	None,
	Forward,
	Backward,
	Left,
	Right,
	ForwardLeft,
	ForwardRight,
	BackwardLeft,
	BackwardRight
};

UCLASS()
class CLAMOR_LIBERTATIS_API APlayerCharacter : public ACharacter, public IPickupItemInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UCombatComponent* CombatComp;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USkillComponent* SkillComp;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AWeaponBase> WeaponClass; 
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AWeaponBase* SpawnedWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UHealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetLock")
	UTargetLockComponent* TargetLockComp;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DeathReactMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* BackwardDodgeReactMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* ForwardDodgeReactMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* LeftDodgeReactMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* RightDodgeReactMontage;

	UPROPERTY(EditAnywhere, Category = "Timer")
	FTimerHandle DodgeTimerHandle;
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent() const { return HealthComp; }
	FVector2D CurrentMoveInput;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* HitEffect;
	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HitSound;

	// 캐릭터 행동 입력
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StopMove(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartBasicAttack(const FInputActionValue& value);
	UFUNCTION()
	void StopBasicAttack(const FInputActionValue& value);
	UFUNCTION()
	void StartDodge(const FInputActionValue& value);
	UFUNCTION()
	void StopDodge(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void StartActiveSkill(const FInputActionValue& value);
	UFUNCTION()
	void Lock(const FInputActionValue& value);
	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnDeath();


	// 캐릭터 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	// 애니메이션 실행
	void HitAnimMontage();
	void DeathAnimMontage();
	void DodgeAnimMontage(EDodgeDirection DodgeDirection);

	void SpawnHitEffect();

	bool IsAvailable();

	EDodgeDirection GetDirection() const;

	//인벤토리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UConsumableInventoryComponent* ConsumableInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<USocketItemInventoryComponent> SocketItemInventory;

	// 아이템 획득
	virtual bool TryPickupItem(class AActor* Item) override;
};
