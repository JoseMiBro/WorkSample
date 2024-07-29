#include "Pestilence/InteractiveActors/IActors/PickUpBase.h"

APickUpBase::APickUpBase()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Default Mesh"));
	Mesh->SetSimulatePhysics(bPhysics);
	RootComponent = Mesh;

	bHolding = false;

	Name = TEXT("DefaultPickUpName");
}

void APickUpBase::DefaultInteraction(AActor* Actor)
{
	bHolding = !bHolding;
	Mesh->SetSimulatePhysics(bHolding ? false : true);
	Mesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if (CurrentInteractor && bHolding)
	{
		Mesh->AttachToComponent(CurrentInteractor->IAHolding, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SetActorLocation(CurrentInteractor->IAHolding->GetComponentLocation());
		CurrentInteractor->CurrentPickUp = this;
		CurrentPicker = CurrentInteractor;
	}
	else if (!bHolding)
	{
		Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		if (bThrowable)
		{
			Mesh->AddForce(CurrentPicker->IAHolding->GetForwardVector() * ThrowForce * Mesh->GetMass());
		}
		CurrentPicker->CurrentPickUp = nullptr;
		CurrentPicker = nullptr;
	}
}
