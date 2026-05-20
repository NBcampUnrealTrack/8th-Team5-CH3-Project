#include "Item/VisualItem/DropItem.h"


ADropItem::ADropItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(SceneComp);
	
	StaticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ADropItem::OnItemBeginOverlap);
	StaticMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}


void ADropItem::BeginPlay()
{
	Super::BeginPlay();
}


void ADropItem::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Item Overlap to %s"),*OtherActor->GetName());
}

void ADropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

