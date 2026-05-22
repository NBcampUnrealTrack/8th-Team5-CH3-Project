//PlayerCharacter.cpp

#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Combat/CombatComponent.h"
#include "Combat/SkillComponent.h"
#include "Combat/Weapon/WeaponBase.h"
#include "Combat/HealthComponent.h"
#include "Character/TargetLockComponent.h"

#include "Blueprint/UserWidget.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // 스프링암, 카메라 컴포넌트 추가 및 설정
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

    SpringArmComp->SetupAttachment(RootComponent);
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    SpringArmComp->TargetArmLength = 300.0f;

    SpringArmComp->bUsePawnControlRotation = true;
    CameraComp->bUsePawnControlRotation = false;

    // 전투 컴포넌트 추가
    CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
    SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));

    // 체력 컴포넌트 추가
    HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

    // 타겟락 컴포넌트 추가
    TargetLockComp = CreateDefaultSubobject<UTargetLockComponent>(TEXT("TargetLockComp"));

    // 캐릭터 이동 속도 설정
    NormalSpeed = 500.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // 캐릭터 회전 설정
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    // 캐릭터 입력, 상태
    CurrentMoveInput = FVector2D::ZeroVector;

    //인벤토리 컴포넌트 추가
    ConsumableInventory = CreateDefaultSubobject<UConsumableInventoryComponent>(TEXT("ConsumableInventory"));
    SocketItemInventory = CreateDefaultSubobject<USocketItemInventoryComponent>(TEXT("SocketItemInventory"));

}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (WeaponClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);

        if (SpawnedWeapon)
        {
            SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Hand_R_Weapon"));
        }
    }
    if (CombatComp && SpawnedWeapon)
    {
        CombatComp->SetCurrentWeapon(SpawnedWeapon);
    }

    ABasePlayerController* PC =
        Cast<ABasePlayerController>(GetController());

    HealthComp->OnDeath.AddDynamic(this, &APlayerCharacter::OnDeath);
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
    if (GetCharacterMovement())
    {
        SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}


void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerCharacter::Move
                );
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Completed,
                    this,
                    &APlayerCharacter::StopMove
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerCharacter::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &APlayerCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &APlayerCharacter::StartSprint
                );
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &APlayerCharacter::StopSprint
                );
            }

            if (PlayerController->BasicAttackAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->BasicAttackAction,
                    ETriggerEvent::Started,
                    this,
                    &APlayerCharacter::StartBasicAttack
                );
                EnhancedInput->BindAction(
                    PlayerController->BasicAttackAction,
                    ETriggerEvent::Completed,
                    this,
                    &APlayerCharacter::StopBasicAttack
                );
            }

            if (PlayerController->DodgeAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->DodgeAction,
                    ETriggerEvent::Started,
                    this,
                    &APlayerCharacter::StartDodge
                );
                //EnhancedInput->BindAction(
                //    PlayerController->DodgeAction,
                //    ETriggerEvent::Completed,
                //    this,
                //    &APlayerCharacter::StopDodge
                //);
            }
            if (PlayerController->ActiveSkillAction_Q)
            {
                EnhancedInput->BindAction(
                    PlayerController->ActiveSkillAction_Q,
                    ETriggerEvent::Started,
                    this,
                    &APlayerCharacter::StartActiveSkill_Q
                );
            }

            if (PlayerController->ActiveSkillAction_E)
            {
                EnhancedInput->BindAction(
                    PlayerController->ActiveSkillAction_E,
                    ETriggerEvent::Started,
                    this,
                    &APlayerCharacter::StartActiveSkill_E
                );
            }

            if (PlayerController->ActiveSkillAction_F)
            {
                EnhancedInput->BindAction(
                    PlayerController->ActiveSkillAction_F,
                    ETriggerEvent::Started,
                    this,
                    &APlayerCharacter::StartActiveSkill_F
                );
            }

            if (PlayerController->LockAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LockAction,
                    ETriggerEvent::Started,
                    this,
                    &APlayerCharacter::Lock
                );
            }
        }
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MoveInput = Value.Get<FVector2D>();
    CurrentMoveInput = MoveInput;

    if (!IsAvailable()) return;

    const FRotator ControlRotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(ForwardDirection, MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(RightDirection, MoveInput.Y);
    }
}

void APlayerCharacter::StopMove(const FInputActionValue& value)
{
    CurrentMoveInput = FVector2D::ZeroVector;
}

void APlayerCharacter::StartJump(const FInputActionValue& value)
{
    if (!IsAvailable()) return;

    if (value.Get<bool>())
    {
        Jump();
    }
}

void APlayerCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();
    if (TargetLockComp && TargetLockComp->GetCurrentTarget())
    {
        // 락온 중에는 마우스가 컨트롤러 회전을 건드리지 않게 막음
        return;
    }
    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void APlayerCharacter::StartBasicAttack(const FInputActionValue& value)
{
    if (!IsAvailable() && !CombatComp->IsAttacking()) return;

    CombatComp->BasicAttack();
}

void APlayerCharacter::StopBasicAttack(const FInputActionValue& value)
{

}

void APlayerCharacter::StartDodge(const FInputActionValue& value)
{
    if (!IsAvailable() && !CombatComp->IsAttacking()) return;
    if (CombatComp->IsInvincible()) return;

    EDodgeDirection DodgeDirection = GetDirection();

    CombatComp->SetCombatState(ECombatEnumState::Dodging);
    CombatComp->SetInvincible(true);

    DodgeAnimMontage(DodgeDirection);
}

void APlayerCharacter::StopDodge(UAnimMontage* Montage, bool bInterrupted)
{
}

void APlayerCharacter::StartActiveSkill_Q(const FInputActionValue& value)
{
    if (!IsAvailable()) return;
    if (SkillDatas.Num() >= 1) {
        UDA_SkillData* SkillData = SkillDatas[0];
        SkillComp->TryActivateSkill(SkillData);
    }
}
void APlayerCharacter::StartActiveSkill_E(const FInputActionValue& value)
{
    if (!IsAvailable()) return;
    if (SkillDatas.Num() >= 1) {
        UDA_SkillData* SkillData = SkillDatas[1];
        SkillComp->TryActivateSkill(SkillData);
    }
}
void APlayerCharacter::StartActiveSkill_F(const FInputActionValue& value)
{
    if (!IsAvailable()) return;
    if (SkillDatas.Num() >= 1) {
        UDA_SkillData* SkillData = SkillDatas[2];
        SkillComp->TryActivateSkill(SkillData);
    }
}

void APlayerCharacter::Lock(const FInputActionValue& value)
{
    if (!TargetLockComp) return;

    TargetLockComp->ToggleLock();
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(
        DamageAmount,
        DamageEvent,
        EventInstigator,
        DamageCauser
    );


    if (HealthComp)
    {
        if (CombatComp->IsInvincible()) {
            UE_LOG(LogTemp, Warning, TEXT("Player is invincible."), ActualDamage);
            return 0.f;
        }

        HealthComp->TakeDamageValue(ActualDamage);
        UE_LOG(LogTemp, Warning, TEXT("Player Took Damage: %f, Current HP: %f"), ActualDamage, HealthComp->GetCurrentHealth());

        if (HealthComp->GetCurrentHealth() > 0.0f && CombatComp->IsSuperarmor() == false)
        {
            HitAnimMontage();
            SpawnHitEffect();
        }
    }
    return ActualDamage;
}

// 사망 시 호출되는 함수
void APlayerCharacter::OnDeath()
{
    if (!CombatComp) return;
    if (CombatComp->IsDead()) return;

    CombatComp->SetCombatState(ECombatEnumState::Dead);
    DeathAnimMontage();
}

// 사망 시 애니메이션 몽타주 실행
void APlayerCharacter::DeathAnimMontage()
{
    if (!DeathReactMontage) return;

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Play(DeathReactMontage);
    }
}

void APlayerCharacter::DodgeAnimMontage(EDodgeDirection DodgeDirection)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance) return;
    AnimInstance->StopAllMontages(0.f);

    UAnimMontage* DodgeMontage = nullptr;
    FRotator CurrentRotate = GetActorRotation();
    float NewRotate = 0.f;
    switch (DodgeDirection)
    {
    case EDodgeDirection::Forward:
        DodgeMontage = ForwardDodgeReactMontage;
        break;
    case EDodgeDirection::Backward:
        DodgeMontage = BackwardDodgeReactMontage;
        break;
    case EDodgeDirection::Left:
        DodgeMontage = LeftDodgeReactMontage;
        break;
    case EDodgeDirection::Right:
        DodgeMontage = RightDodgeReactMontage;
        break;
    case EDodgeDirection::ForwardLeft:
        NewRotate = 45.f;
        DodgeMontage = LeftDodgeReactMontage;
        break;
    case EDodgeDirection::ForwardRight:
        NewRotate = -45.f;
        DodgeMontage = RightDodgeReactMontage;
        break;
    case EDodgeDirection::BackwardLeft:
        NewRotate = 45.f;
        DodgeMontage = BackwardDodgeReactMontage;
        break;
    case EDodgeDirection::BackwardRight:
        NewRotate = -45.f;
        DodgeMontage = BackwardDodgeReactMontage;
        break;
    default:
        DodgeMontage = BackwardDodgeReactMontage;
        break;
    }

    if (!DodgeMontage)
    {
        return;
    }
    CurrentRotate.Yaw += NewRotate;
    SetActorRotation(CurrentRotate);
    AnimInstance->Montage_Play(DodgeMontage);

    if (TargetLockComp) {
        TargetLockComp->ToggleCharacterRotationLock(false);
    }
}

// 피격 시 애니메이션 몽타주 실행
void APlayerCharacter::HitAnimMontage()
{
    if (!HitReactMontage) return;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(HitReactMontage);
    }
}

bool APlayerCharacter::IsAvailable()
{
    if (!CombatComp) return false;
    if (!HealthComp) return false;
    if (GetCharacterMovement()->IsFalling()) return false;
    if (!CombatComp->IsIdle()) return false;

    return true;
}

EDodgeDirection APlayerCharacter::GetDirection() const
{
    if (CurrentMoveInput.IsNearlyZero())
    {
        return EDodgeDirection::None;
    }

    const float DirectionAngle = FMath::RadiansToDegrees(FMath::Atan2(CurrentMoveInput.Y, CurrentMoveInput.X));
    bool IsAttacking = CombatComp->IsAttacking();
    if (TargetLockComp->GetCurrentTarget() == nullptr && !IsAttacking) {
        //타겟이 없다면 원하는 방향으로 바라보고 이동하니 forward 방향 반환함.
        return EDodgeDirection::Forward;
    }
    if (DirectionAngle >= -22.5f && DirectionAngle < 22.5f)
    {
        return EDodgeDirection::Forward;
    }
    if (DirectionAngle >= 22.5f && DirectionAngle < 67.5f)
    {
        return EDodgeDirection::ForwardRight;
    }
    if (DirectionAngle >= 67.5f && DirectionAngle < 112.5f)
    {
        return EDodgeDirection::Right;
    }
    if (DirectionAngle >= 112.5f && DirectionAngle < 157.5f)
    {
        return EDodgeDirection::BackwardRight;
    }
    if (DirectionAngle >= -67.5f && DirectionAngle < -22.5f)
    {
        return EDodgeDirection::ForwardLeft;
    }
    if (DirectionAngle >= -112.5f && DirectionAngle < -67.5f)
    {
        return EDodgeDirection::Left;
    }
    if (DirectionAngle >= -157.5f && DirectionAngle < -112.5f)
    {
        return EDodgeDirection::BackwardLeft;
    }

    return EDodgeDirection::Backward;
}

void APlayerCharacter::SpawnHitEffect()
{
    if (HitEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            HitEffect,
            GetActorLocation(),
            GetActorRotation(),
            FVector(1.0f)
        );
    }

    if (HitCameraShakeClass)
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
            if (PlayerController->PlayerCameraManager)
            {
                PlayerController->PlayerCameraManager->StartCameraShake(HitCameraShakeClass, 1.0f);
            }
        }
    }

    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            HitSound,
            GetActorLocation(),
            1.0f,
            1.0f,
            0.0f
        );
    }
}

void APlayerCharacter::PickupItem(FItemTableRow* ItemData)
{
    if (!ItemData) return;

    if (ItemData->ItemType == EItemType::Consumable && ConsumableInventory && ConsumableInventory->ItemDataTable)
    {
        TArray<FName> RowNames = ConsumableInventory->ItemDataTable->GetRowNames();
        for (const FName& Name : RowNames)
        {
            if (ConsumableInventory->ItemDataTable->FindRow<FItemTableRow>(Name, TEXT("")) == ItemData)
            {
                ConsumableInventory->AddItem(Name, 1);
                UE_LOG(LogTemp, Warning, TEXT("소비 아이템 획득: %s"), *Name.ToString());
                return;
            }
        }
    }

    else if (ItemData->ItemType == EItemType::SocketItem && SocketItemInventory && SocketItemInventory->ItemDataTable)
    {
        TArray<FName> RowNames = SocketItemInventory->ItemDataTable->GetRowNames();
        for (const FName& Name : RowNames)
        {
            if (SocketItemInventory->ItemDataTable->FindRow<FItemTableRow>(Name, TEXT("")) == ItemData)
            {
                SocketItemInventory->AddItem(Name, 1);
                UE_LOG(LogTemp, Warning, TEXT("소켓 아이템 획득: %s"), *Name.ToString());
                return;
            }
        }
    }
}