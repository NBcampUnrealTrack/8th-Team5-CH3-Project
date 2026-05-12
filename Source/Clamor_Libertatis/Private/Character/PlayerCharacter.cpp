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
    if (!IsAvailable())
    {
        if (!CombatComp->IsAttacking()) return;
    }

    CombatComp->BasicAttack();
}

void APlayerCharacter::StopBasicAttack(const FInputActionValue& value)
{

}

void APlayerCharacter::StartDodge(const FInputActionValue& value)
{
    if (!IsAvailable()) return;
    if (CombatComp->IsInvincible()) return;

    CombatComp->SetCombatState(ECombatEnumState::Dodging);

    //뱡향키와 회피키 동시 입력했을 경우 해당 방향으로 덤블링
    if (CurrentMoveInput.X || CurrentMoveInput.Y)
    {
        ForwardDodgeAnimMontage();
    }
    // 회피키만 입력했을 경우 백덤블링
    else
    {
        BackDodgeAnimMontage();
    }
}

void APlayerCharacter::StopDodge(UAnimMontage* Montage, bool bInterrupted)
{
}

void APlayerCharacter::StartActiveSkill(const FInputActionValue& value)
{
    if (!IsAvailable()) return;

    SkillComp->ActiveSkill();
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
            CombatComp->HitReact(true);
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

void APlayerCharacter::BackDodgeAnimMontage()
{
    if (!BackDodgeReactMontage) return;

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Play(BackDodgeReactMontage);

        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &APlayerCharacter::DodgeMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, BackDodgeReactMontage);
    }
}

void APlayerCharacter::ForwardDodgeAnimMontage()
{
    if (!ForwardDodgeReactMontage) return;

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Play(ForwardDodgeReactMontage);

        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &APlayerCharacter::DodgeMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, ForwardDodgeReactMontage);
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

//회피 애니메이션 비정상 종료 시 EndDodge 호출
void APlayerCharacter::DodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
    {
        CombatComp->EndDodge();
        return;
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