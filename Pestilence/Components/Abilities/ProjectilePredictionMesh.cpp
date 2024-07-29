#include "ProjectilePredictionMesh.h"

AProjectilePredictionMesh::AProjectilePredictionMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default Mesh"));
	DefaultMesh->SetupAttachment(RootComponent);

	SetVisible(false);
}

void AProjectilePredictionMesh::SetVisible(bool _bVisible)
{
	DefaultMesh->SetVisibility(_bVisible, true);
}
