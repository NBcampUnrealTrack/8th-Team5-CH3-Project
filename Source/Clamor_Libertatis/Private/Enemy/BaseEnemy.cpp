#include "Enemy/BaseEnemy.h"
#include "Components/WidgetComponent.h"
#include "UI/DamageTextWidget.h"
#include "BrainComponent.h"
#include "Enemy/ActorComponent/Enemy_CombatComponent.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Enemy/AIController/Enemy_AIController.h"
#include "Enemy/Animations/BaseEnemyAnimInst.h"
#include "Enemy/E_Weapon/Enemy_BaseWeapon.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Enemy_StatComp = CreateDefaultSubobject<UEnemy_StatComponent>(TEXT("StatComponent"));
	
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	HPWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	HPWidgetComp->SetupAttachment(GetRootComponent());
	HPWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	
	Enemy_CombatComp = CreateDefaultSubobject<UEnemy_CombatComponent>(TEXT("CombatComponent"));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	EquipWeapon();
	if (Enemy_StatComp)
	{
		UE_LOG(LogTemp,Warning,TEXT("Initialized Stat"));
		Enemy_StatComp->InitializeStat();

		if (HealthComp)
		{
			HealthComp->SetMaxHealth(Enemy_StatComp->GetEnemyStat().HP);
		}

	}
	if (AEnemy_AIController* AIC = Cast<AEnemy_AIController>(GetController()))
	{
		AIC->InitializeAIPerceptionComponent();
	}
	if (GetMesh()->GetAnimInstance())
	{
		AnimInst = Cast<UBaseEnemyAnimInst>(GetMesh()->GetAnimInstance());
	}

	if (HPWidgetComp)
	{
		UUserWidget* HPWidget = HPWidgetComp->GetUserWidgetObject();

		if (HPWidget)
		{
			FString FunctionName = TEXT("InitWidget");
			UFunction* Func = HPWidget->FindFunction(*FunctionName);
			if (Func)
			{
				struct FInitWidgetParams
				{
					UHealthComponent* InHealthComp;
				};
				FInitWidgetParams Params;
				Params.InHealthComp = HealthComp;

				HPWidget->ProcessEvent(Func, &Params);
				UE_LOG(LogTemp, Warning, TEXT("HP Widget Initialized Successfully"));
			}
		}
	}
}

void ABaseEnemy::AttackToPlayer()
{
	UE_LOG(LogTemp,Warning,TEXT("Enemy Attack Started"));
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::EquipWeapon()
{
	if (Enemy_WeaponClass)
	{
		Enemy_WeaponInst = Cast<AEnemy_BaseWeapon>(GetWorld()->SpawnActor<AActor>(Enemy_WeaponClass,FVector::ZeroVector,FRotator::ZeroRotator));
		if (Enemy_WeaponInst)
		{
			FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		
			Enemy_WeaponInst->AttachToComponent(GetMesh(),AttachmentTransformRules,TEXT("WeaponSocket"));
			UE_LOG(LogTemp,Warning,TEXT("Enemy Weapon Initialized"));
		}
	}
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Enemy_StatComp)
	{
		if (Enemy_StatComp->GetEnemyStat().SetHP(ActualDamage))
		{
			// Enemy is Not Dead
		}
		else
		{
			OnDead();
		}
	}

	if (HealthComp)
	{
		HealthComp->TakeDamageValue(DamageAmount);
	}

	if (DamageTextActorClass)
	{
		FVector SpawnLocation = GetActorLocation()
			+ GetActorForwardVector() * 50.f
			+ FVector(0.f, 0.f, 100.f);

		AActor* DamageActor = GetWorld()->SpawnActor<AActor>(
			DamageTextActorClass,
			SpawnLocation,
			FRotator::ZeroRotator
		);

		if (DamageActor)
		{
			UWidgetComponent* WidgetComp =
				DamageActor->FindComponentByClass<UWidgetComponent>();

			if (WidgetComp)
			{
				UDamageTextWidget* DamageWidget =
					Cast<UDamageTextWidget>(
						WidgetComp->GetUserWidgetObject()
					);

				if (DamageWidget)
				{
					DamageWidget->InitDamageText(DamageAmount);
				}
			}

			DamageActor->SetLifeSpan(2.f);
		}
	}
	return DamageAmount;
}

void ABaseEnemy::AttackHitCheck()
{
	UE_LOG(LogTemp,Warning,TEXT("AttackHitCheckOn"));
}

void ABaseEnemy::OnDead()
{
	UE_LOG(LogTemp,Warning,TEXT("%s Was Dead"),*GetName());
	
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC && AIC->GetBrainComponent())
	{
		AIC->GetBrainComponent()->StopLogic(TEXT("Because Owner Was Dead"));
	}
	
	UAnimMontage* AM_Dead = Enemy_CombatComp->DA_EnemyAnim->AM_Dead;
	if (AnimInst && AM_Dead)
	{
		AnimInst->Montage_Play(AM_Dead);
	}
	
	SetLifeSpan(4.f);
}

void ABaseEnemy::Destroyed()
{
	if (Enemy_WeaponInst)
	{
		Enemy_WeaponInst->Destroy();
	}
	
	Super::Destroyed();
}
