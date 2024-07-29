#include "Pestilence/InteractiveActors/InteractiveActor.h"

AInteractiveActor::AInteractiveActor()
{
	//PrimaryActorTick.bCanEverTick = true;
	//DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default Mesh"));
	CurrentInteractor = nullptr;
}

void AInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveActor::DefaultInteraction(AActor* Actor)
{
	if (CurrentInteractor)
	{
	}
}

