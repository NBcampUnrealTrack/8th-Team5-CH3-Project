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
	
// 	UPROPERTY(EditAnywhere, Category = "Effects")
//     UNiagaraComponent* MagicProjectileEffect;
	
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

	FTimerHandle CollisionEnableTimerHandle;
};
