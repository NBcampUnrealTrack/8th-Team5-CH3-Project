//PlayerCharacter.cpp

#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

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
            if (PlayerController->ActiveSkillAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->ActiveSkillAction,
                    ETriggerEvent::Started,
                    this,
                    &APlayerCharacter::StartActiveSkill
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
    if (!IsAvailable()) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();
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
    CurrentMoveInput = MoveInput;
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

    CombatComp->SetCombatState(ECombatEnumState::Dodging);
    CombatComp->SetInvincible(true);

    DodgeAnimMontage(GetDirection());
}

void APlayerCharacter::StopDodge(UAnimMontage* Montage, bool bInterrupted)
{
}

void APlayerCharacter::StartActiveSkill(const FInputActionValue& value)
{
    if (!IsAvailable()) return;

    SkillComp->ActiveSkill();
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
    UE_LOG(LogTemp, Warning, TEXT("Player Took Damage: %f"), ActualDamage);

    if (HealthComp)
    {
        if (CombatComp->IsInvincible())
            return 0.f;

        HealthComp->TakeDamageValue(ActualDamage);

        UE_LOG(LogTemp, Warning, TEXT("Current HP: %f"),
            HealthComp->GetCurrentHealth());

        if (HealthComp->GetCurrentHealth() > 0.0f)
        {
            HitAnimMontage();
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

    switch (DodgeDirection)
    {
    case EDodgeDirection::Forward:
        if (!ForwardDodgeReactMontage) return;
        AnimInstance->Montage_Play(ForwardDodgeReactMontage);
        break;
    case EDodgeDirection::Backward:
        if (!BackwardDodgeReactMontage) return;
        AnimInstance->Montage_Play(BackwardDodgeReactMontage);
        break;
    case EDodgeDirection::Left:
        if (!LeftDodgeReactMontage) return;
        AnimInstance->Montage_Play(LeftDodgeReactMontage);
        break;
    case EDodgeDirection::Right:
        if (!RightDodgeReactMontage) return;
        AnimInstance->Montage_Play(RightDodgeReactMontage);
        break;
    default:
        AnimInstance->Montage_Play(ForwardDodgeReactMontage);
        break;
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

        //FOnMontageEnded EndDelegate;
        //EndDelegate.BindUObject(this, &APlayerCharacter::HitMontageEnded);
        //AnimInstance->Montage_SetEndDelegate(EndDelegate, HitReactMontage);
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
    if (CurrentMoveInput.IsNearlyZero())  return EDodgeDirection::None;
    if (FMath::Abs(CurrentMoveInput.X) >= FMath::Abs(CurrentMoveInput.Y))
        if (CurrentMoveInput.X >= 0) return EDodgeDirection::Forward;
        else return EDodgeDirection::Backward;
    if (FMath::Abs(CurrentMoveInput.X) < FMath::Abs(CurrentMoveInput.Y))
        if (CurrentMoveInput.Y >= 0) return EDodgeDirection::Right;
        else return EDodgeDirection::Left;
    return EDodgeDirection::None;
}