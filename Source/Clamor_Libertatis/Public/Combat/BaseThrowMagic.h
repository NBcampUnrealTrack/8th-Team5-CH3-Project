// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseThrowMagic.generated.h"

class USphereComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;
class UNiagaraComponent;

UCLASS()
class CLAMOR_LIBERTATIS_API ABaseThrowMagic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseThrowMagic();

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraSystem> MagicHitEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic|Attack")
	float DamageAmount = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic|Attack")
	float ExplosionRadius = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic|Attack")
	float EffectScale = 1.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic|Projectile")
	float InitialSpeed = 1500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic|Projectile")
	float MaxSpeed = 1500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic|Projectile")
	float LifeTime = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Magic|Projectile")
	float CollisionEnableDelay = 0.05f;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	void ApplyExplosionDamage(const FVector& ExplosionLocation);

	void EnableCollision();

	// [권기문] 서브클래스에서 오버라이드하여 히트 반응 대상을 필터링할 수 있도록 추가
	// 기본값 true = 기존 동작 유지 (모든 액터에 반응)
	// EnemyThrowMagic에서 Player 태그 액터만 true를 반환하도록 재정의하여 사용 중
	virtual bool ShouldReactToHit(AActor* OtherActor) const { return true; }

	FTimerHandle CollisionEnableTimerHandle;

public:
	void PrepareForLaunch();
	void Launch(FVector Direction);
};
