#include "Enemy/AIController/Enemy_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemy_AIController::AEnemy_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	Sight_Config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));
}

void AEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemy_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(),GetPawn()->GetActorLocation(),Sight_Config->SightRadius,32,FColor::Green);
	DrawDebugSphere(GetWorld(),GetPawn()->GetActorLocation(),Sight_Config->LoseSightRadius,32,FColor::Red);
	if (ABaseEnemy* MyOwner = Cast<ABaseEnemy>(GetPawn()))
	{
		float MaxChasingDistance = MyOwner->GetEnemyStatComp()->GetEnemyStat().MaxChasingDistance;
		DrawDebugSphere(GetWorld(),GetPawn()->GetActorLocation(),MaxChasingDistance,32,FColor::Blue);
	}
}

void AEnemy_AIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor->ActorHasTag(TEXT("Player")))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp,Warning,TEXT("Player 감지 성공"));
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"),Actor);
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Player 감지 실패"));
			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"),nullptr);
		}
	}
}

void AEnemy_AIController::InitializeAIPerceptionComponent()
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GetPawn()))
	{
		if (Enemy->GetEnemyStatComp())
		{
			Sight_Config->SightRadius = Enemy->GetEnemyStatComp()->GetEnemyStat().SightRadius;
			Sight_Config->LoseSightRadius = Enemy->GetEnemyStatComp()->GetEnemyStat().LoseSightRadius;
			Sight_Config->PeripheralVisionAngleDegrees = Enemy->GetEnemyStatComp()->GetEnemyStat().PeripheralVisionAngleDegrees;
			Sight_Config->SetMaxAge(Enemy->GetEnemyStatComp()->GetEnemyStat().SightConfig_MaxAge);
	
			Sight_Config->DetectionByAffiliation.bDetectEnemies = true;
			Sight_Config->DetectionByAffiliation.bDetectFriendlies = true;
			Sight_Config->DetectionByAffiliation.bDetectNeutrals = true;
			
			AIPerceptionComp->ConfigureSense(*Sight_Config);
			AIPerceptionComp->RequestStimuliListenerUpdate();
			AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemy_AIController::OnTargetPerceived);
		}
		if (RunBehaviorTree(BT_BaseEnemy))
		{
			UE_LOG(LogTemp,Warning,TEXT("Enemy BehaviorTree Run"));
		}
	}
}

