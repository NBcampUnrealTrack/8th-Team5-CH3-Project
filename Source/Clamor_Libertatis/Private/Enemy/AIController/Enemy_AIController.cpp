#include "Enemy/AIController/Enemy_AIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemy_AIController::AEnemy_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	Sight_Config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));
	Sight_Config->SightRadius = 800.f;
	Sight_Config->LoseSightRadius = 1000.f;
	Sight_Config->PeripheralVisionAngleDegrees = 180.f;
	Sight_Config->SetMaxAge(5.f);
	
	Sight_Config->DetectionByAffiliation.bDetectEnemies = true;
	Sight_Config->DetectionByAffiliation.bDetectFriendlies = true;
	Sight_Config->DetectionByAffiliation.bDetectNeutrals = true;
	
	AIPerceptionComp->ConfigureSense(*Sight_Config);
	AIPerceptionComp->RequestStimuliListenerUpdate();
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemy_AIController::OnTargetPerceived);
}


void AEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(),GetPawn()->GetActorLocation(),Sight_Config->SightRadius,32,FColor::Green);
	DrawDebugSphere(GetWorld(),GetPawn()->GetActorLocation(),Sight_Config->LoseSightRadius,32,FColor::Red);
}


void AEnemy_AIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor->ActorHasTag(TEXT("Player")))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp,Warning,TEXT("Player 감지 성공"));
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Player 감지 실패"));
		}
	}
}



