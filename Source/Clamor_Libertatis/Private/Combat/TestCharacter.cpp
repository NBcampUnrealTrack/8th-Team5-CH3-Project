// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TestCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
//전투
#include "Combat/CombatTestPlayerController.h"
#include "Combat/CombatComponent.h"
#include "Combat/Weapon/WeaponBase.h"
#include "Combat/HealthComponent.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//스프링암을 루트컴포넌트에 부착하고
	SpringArmComp->SetupAttachment(RootComponent);
	//캐릭터와 거리를 300
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//카메라를 스프링암에 부착, 다만 소캣에 부착하는데 이는 스프링암에 끝부분에 부착하는걸 의미함.
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(
			WeaponClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (EquippedWeapon)
		{
			EquippedWeapon->AttachToCharacterHand(this);

			if (CombatComponent)
			{
				CombatComponent->SetCurrentWeapon(EquippedWeapon);
			}
		}
	}
}
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (ACombatTestPlayerController* PlayerController = Cast<ACombatTestPlayerController>(GetController())) {
			//이동
			if (PlayerController->MoveAction) {
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::Move
				);
			}
			//점프 시작
			if (PlayerController->LookAction) {
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ATestCharacter::Look
				);
			}
			//점프 시작
			if (PlayerController->BasicAttackAction) {
				EnhancedInput->BindAction(
					PlayerController->BasicAttackAction,
					ETriggerEvent::Started,
					this,
					&ATestCharacter::BasicAttack
				);
			}
		}
	}
}

float ATestCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(
		DamageAmount,
		DamageEvent,
		EventInstigator,
		DamageCauser
	);

	if (HealthComponent)
	{
		HealthComponent->TakeDamageValue(ActualDamage);

	}

	return ActualDamage;
}

void ATestCharacter::Move(const FInputActionValue& value) {
	if (!Controller)return;
	const FVector2D moveInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(moveInput.X)) {
		AddMovementInput(GetActorForwardVector(), moveInput.X);
	}
	if (!FMath::IsNearlyZero(moveInput.Y)) {
		AddMovementInput(GetActorRightVector(), moveInput.Y);
	}
}
void ATestCharacter::Look(const FInputActionValue& value) {
	FVector2D LookAxisVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATestCharacter::BasicAttack(const FInputActionValue& value)
{
	CombatComponent->BasicAttack();
}
