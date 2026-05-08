// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BaseThrowMagic.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseThrowMagic::ABaseThrowMagic()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    SetRootComponent(SphereComponent);

    SphereComponent->InitSphereRadius(20.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);

    SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

    SphereComponent->SetNotifyRigidBodyCollision(true);
    SphereComponent->SetGenerateOverlapEvents(false);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileEffect"));
	NiagaraComponent->SetupAttachment(SphereComponent);
	NiagaraComponent->SetAutoActivate(true);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->UpdatedComponent = SphereComponent;
	
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

    InitialLifeSpan = LifeTime;
}

// Called when the game starts or when spawned
void ABaseThrowMagic::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentHit.AddDynamic(this, &ABaseThrowMagic::OnProjectileHit);

	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;


    //생성후 CollisionEnableDelay초동안 충돌X
    if (CollisionEnableDelay <= 0.0f)
    {
        EnableCollision();
        return;
    }

    GetWorldTimerManager().SetTimer(
        CollisionEnableTimerHandle,
        this,
        &ABaseThrowMagic::EnableCollision,
        CollisionEnableDelay,
        false
    );
}

void ABaseThrowMagic::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

    const FVector ExplosionLocation = Hit.ImpactPoint;

    if (MagicHitEffect)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            MagicHitEffect,
            ExplosionLocation,
            Hit.ImpactNormal.Rotation(),
            FVector(EffectScale)
        );
        if (NiagaraComp)
        {
            //크기 파라미터 즉시수정.
            NiagaraComp->SetVariableFloat(TEXT("Scale_All"), EffectScale);
        }
    }

    ApplyExplosionDamage(ExplosionLocation);
	Destroy();
}
void ABaseThrowMagic::ApplyExplosionDamage(const FVector& ExplosionLocation)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(this);

    if (AActor* OwnerActor = GetOwner())
    {
        IgnoreActors.Add(OwnerActor);
    }

    if (GetInstigator())
    {
        IgnoreActors.Add(GetInstigator());
    }

    TArray<FHitResult> HitResults;

    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(ExplosionRadius);

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActors(IgnoreActors);
    QueryParams.bTraceComplex = false;

    const bool bHit = World->SweepMultiByChannel(
        HitResults,
        ExplosionLocation,
        ExplosionLocation,
        FQuat::Identity,
        ECC_Pawn,//<-Pawn 만 체크하는거임
        CollisionShape,
        QueryParams
    );
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    DrawDebugSphere(
        World,
        ExplosionLocation,
        ExplosionRadius,
        32,
        FColor::Red,
        false,
        1.0f
    );
#endif

    if (!bHit)
    {
        return;
    }

    TSet<AActor*> DamagedActors;

    AController* InstigatorController = GetInstigatorController();

    for (const FHitResult& HitResult : HitResults)
    {
        AActor* HitActor = HitResult.GetActor();

        if (!HitActor)
        {
            continue;
        }

        if (DamagedActors.Contains(HitActor))
        {
            continue;
        }

        DamagedActors.Add(HitActor);

        UGameplayStatics::ApplyDamage(
            HitActor,
            DamageAmount,
            InstigatorController,
            this,
            UDamageType::StaticClass()
        );
    }
}

void ABaseThrowMagic::EnableCollision()
{
    if (SphereComponent)
    {
        SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}
