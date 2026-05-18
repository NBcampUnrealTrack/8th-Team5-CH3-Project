// TargetLockComponent.cpp

#include "Character/TargetLockComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"

UTargetLockComponent::UTargetLockComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = false;

    TraceRadius = 150.f;
    MaxLockDistsq = 1000.f * 1000.f * 1.3f;
}


void UTargetLockComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UTargetLockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (CurrentTarget)
    {
        UpdateRotation(DeltaTime);
    }
}

void UTargetLockComponent::ToggleLock()
{
    if (CurrentTarget)
    {
        EndLock();
        return;
    }

    StartLock();
}

void UTargetLockComponent::ToggleCharacterRotationLock(bool bEnable)
{
    //다른 동작중 캐릭터의 회전이 고정되는것을 조절하기위한 함수, CurrentTarget을 유지함.
    if (!CurrentTarget)
        return;
    if (CurrentTarget && OwnerCharacter)
    {
        if (UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement())
        {
			// 캐릭터 정면 고정
            OwnerCharacter->bUseControllerRotationYaw = bEnable;
        }
    }
}
void UTargetLockComponent::StartLock()
{
    CurrentTarget = Cast<ABaseEnemy>(FindTarget());

    if (CurrentTarget && OwnerCharacter)
    {
        if (UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement())
        {
            // 캐릭터 정면 고정
            OwnerCharacter->bUseControllerRotationYaw = true;
        }
        SetComponentTickEnabled(true);
        OnLockTargetChanged.Broadcast(CurrentTarget);
    }
}

void UTargetLockComponent::EndLock()
{
    CurrentTarget = nullptr;
    OnLockTargetChanged.Broadcast(nullptr);

    if (OwnerCharacter)
    {
        if (UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement())
        {
            OwnerCharacter->bUseControllerRotationYaw = false;
        }
    }
    SetComponentTickEnabled(false);
}

AActor* UTargetLockComponent::FindTarget()
{
    if (!OwnerCharacter) return nullptr;

    FVector Start = OwnerCharacter->GetActorLocation();
    FVector End = Start + OwnerCharacter->GetController()->GetControlRotation().Vector() * 1000.0f;
    TArray<FHitResult> HitResults;

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerCharacter);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

    bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        this,
        Start,
        End,
        TraceRadius,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration,
        HitResults,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        2.f
    );

    if (!bHit) return nullptr;
    float MinDistsq = TNumericLimits<float>::Max();
    AActor* BestTarget = nullptr;

    for (const FHitResult& Hit : HitResults)
    {
        AActor* Target = Hit.GetActor();
        if (IsValidTarget(Target))
        {
            // 플레이어와 가장 가까운 거리의 타겟을 구함
            float DistSq = FVector::DistSquaredXY(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
            if (DistSq < MinDistsq)
            {
                MinDistsq = DistSq;
                BestTarget = Target;
            }
        }
    }
    return BestTarget;
}

// 타겟이 유효한지, Enemy인지 검사
bool UTargetLockComponent::IsValidTarget(AActor* Target) const
{
    if (!Target) return false;
    if (!OwnerCharacter) return false;

    if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Target)) return true;

    return false;
}

void UTargetLockComponent::UpdateRotation(float DeltaTime)
{
    if (!CurrentTarget || !OwnerCharacter || !OwnerCharacter->GetController()) return;

    FVector Start = OwnerCharacter->GetActorLocation();
    FVector End = CurrentTarget->GetActorLocation();
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
    TargetRotation.Pitch = -15.f;

    FRotator CurrentRotation = OwnerCharacter->GetController()->GetControlRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.f);
    OwnerCharacter->GetController()->SetControlRotation(NewRotation);
    if (!ValidateCurrentTarget())
    {
        EndLock();
    }
}

bool UTargetLockComponent::ValidateCurrentTarget() const
{
    if (OwnerCharacter && CurrentTarget)
    {
        // 적이 죽거나 일정 거리 이상 멀어질 경우 false
        if (!CurrentTarget->IsDead())
        {
            if (MaxLockDistsq > FVector::DistSquaredXY(OwnerCharacter->GetActorLocation(), CurrentTarget->GetActorLocation()))
            {
                return true;
            }
        }
    }
    return false;
}

ABaseEnemy* UTargetLockComponent::GetCurrentTarget() const
{
    return CurrentTarget;
}
