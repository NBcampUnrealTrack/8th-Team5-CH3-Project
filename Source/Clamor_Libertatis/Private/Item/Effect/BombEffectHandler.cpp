#include "Item/Effect/BombEffectHandler.h"
#include "Enemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

bool UBombEffectHandler::Execute_Implementation(AActor* Owner, const FItemTableRow& ItemData)
{
    if (!Owner) return false;

    UWorld* World = Owner->GetWorld();
    if (!World) return false;

    FVector Origin = Owner->GetActorLocation();
    float Radius = ItemData.EffectRadius;
    float Damage = ItemData.EffectValue;
    float StunDuration = ItemData.EffectDuration;

    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    World->OverlapMultiByChannel(
        Overlaps,
        Origin,
        FQuat::Identity,
        ECC_Pawn,
        Sphere,
        Params
    );

    DrawDebugSphere(World, Origin, Radius, 16, FColor::Red, false, 2.f);

    for (const FOverlapResult& Overlap : Overlaps)
    {
        AActor* HitActor = Overlap.GetActor();
        if (!HitActor || HitActor == Owner) continue;

        UGameplayStatics::ApplyDamage(
            HitActor,
            Damage,
            Owner->GetInstigatorController(),
            Owner,
            UDamageType::StaticClass()
        );

        ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitActor);
        if (Enemy)
            Enemy->ApplyStun(StunDuration);

        UE_LOG(LogTemp, Warning, TEXT("ÆøÅº È÷Æ®: %s, µ¥¹ÌÁö: %f, ½ºÅÏ: %fÃÊ"),
            *HitActor->GetName(), Damage, StunDuration);
    }

    return true;
}