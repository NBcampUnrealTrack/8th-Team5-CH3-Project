//PlayerCharacter.cpp

#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/BasePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Combat/CombatTestPlayerController.h"
#include "Combat/CombatComponent.h"
#include "Combat/Weapon/WeaponBase.h"
#include "Combat/HealthComponent.h"

#include "Blueprint/UserWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
    IsDead = false;
    IsHurt = false;

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
    SpringArmComp->bUsePawnControlRotation = true;
    CameraComp->bUsePawnControlRotation = false;

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
                EnhancedInput->BindAction(
                    PlayerController->DodgeAction,
                    ETriggerEvent::Completed,
                    this,
                    &APlayerCharacter::StopDodge
                );
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
    if (!Controller) return;
    if (IsHurt) return ;
    if (IsDead) return;

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
}

void APlayerCharacter::StartJump(const FInputActionValue& value)
{
    if (IsHurt) return;
    if (IsDead) return;

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
    if (IsHurt) return;
    if (IsDead) return;

    if (!CombatComp) return;
    CombatComp->BasicAttack();
}

void APlayerCharacter::StopBasicAttack(const FInputActionValue& value)
{

}

void APlayerCharacter::StartDodge(const FInputActionValue& value)
{

}

void APlayerCharacter::StopDodge(const FInputActionValue& value)
{

}

void APlayerCharacter::StartActiveSkill(const FInputActionValue& value)
{
    CombatComp->ActiveSkill();
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
        HealthComp->TakeDamageValue(ActualDamage);

        UE_LOG(LogTemp, Warning, TEXT("Current HP: %f"),
            HealthComp->CurrentHealth);

        if (HealthComp->CurrentHealth <= 0.0f)
        {
            OnDead();
        }
        else
        {
            HitAnimMontage();
        }
    }
    return ActualDamage;
}

// 사망 시 호출되는 함수
void APlayerCharacter::OnDead()
{
    if (IsDead) return;
    IsDead = true; // 캐릭터 입력 차단
    
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

// 피격 시 애니메이션 몽타주 실행
void APlayerCharacter::HitAnimMontage()
{
    if (!HitReactMontage) return;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(HitReactMontage);

        IsHurt = true; // 피격 애니메이션 끝나기 전까지 입력 차단

        FOnMontageEnded EndDelegate;
        EndDelegate.BindUObject(this, &APlayerCharacter::HitMontageEnded);
        AnimInstance->Montage_SetEndDelegate(EndDelegate, HitReactMontage);
    }
}


// 피격 애니메이션이 끝나면 입력 허용
void APlayerCharacter::HitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    IsHurt = false;
}

