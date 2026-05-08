//PlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCombatComponent;
class UHealthComponent;
class AWeaponBase;
struct FInputActionValue;

class UHealthComponent;
class UPlayerHUDWidget;


UCLASS()
class CLAMOR_LIBERTATIS_API APlayerCharacter : public ACharacter
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

	// 카메라, 스프링암 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UCombatComponent* CombatComp;
	UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<AWeaponBase> WeaponClass; 
    UPROPERTY(VisibleAnywhere, Category = "Combat")
    AWeaponBase* SpawnedWeapon;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
	UHealthComponent* HealthComp;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitReactMontage;
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent() const{return HealthComp;}


	// 캐릭터 행동 입력
	UFUNCTION()
	void Move(const FInputActionValue& value);
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
	void StopDodge(const FInputActionValue& value);
	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnDead();
	UFUNCTION()
	void HitMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 캐릭터 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;
	bool IsDead;
	bool IsHurt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidget;

	// 피격 애니메이션 실행
	void HitAnimMontage();

};
