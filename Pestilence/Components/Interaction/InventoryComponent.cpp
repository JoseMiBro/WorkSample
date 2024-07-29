#include "InventoryComponent.h"
#include "InteractableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Characters/PestilenceCharacter.h"
#include "InteractorComponent.h"


// -----------------------------------------

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// -----------------------------------------

void UInventoryComponent::Initialize(USceneComponent* InventoryAttachment)
{
	if (!ensure(InventoryAttachment))
		return;
	PickableAttachment = InventoryAttachment;
}

// -----------------------------------------

void UInventoryComponent::TakePickable(UPickableComponent* Pickable)
{
	if (HasPickable())
	{
		OnTakePickable.Broadcast(Pickable, false);
		return;
	}
	
	AActor* PickableOwner = Pickable->GetOwner();
	
	if (!ensure(PickableOwner) || !ensure(PickableAttachment))
	{
		OnTakePickable.Broadcast(Pickable, false);
		return;
	}

  UGameplayStatics::PlaySoundAtLocation(this, Pickable->PickSound, PickableOwner->GetActorLocation());
  Pickable->GetInteractable()->SetNoMaterialOverlay();
	Pickable->GetInteractable()->SetCanBeInteracted(false);

	Pickable->SetActive(false, false);
	
	if (auto* PickableMeshBase = PickableOwner->GetComponentByClass(UStaticMeshComponent::StaticClass()))
	{
		UStaticMeshComponent* PickableMesh = Cast<UStaticMeshComponent>(PickableMeshBase);
		PickableMesh->SetSimulatePhysics(false);
		PickableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	PickableOwner->AttachToComponent(PickableAttachment,{
		EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true
	}); //TODO Sockets

	PickableOwner->SetActorRelativeLocation(FVector::Zero());

	CurrentPickable = Pickable;
	
	OnTakePickable.Broadcast(Pickable, true);

  APestilenceCharacter* Character = Cast<APestilenceCharacter>(GetOwner());
  if (!Character) return;
  Character->GetInteractorComponent()->Enable(false);
}

// -----------------------------------------

void UInventoryComponent::DropPickable(bool bLaunch)
{
	if (!CurrentPickable || !ensure(PickableAttachment))
		return;

	AActor* PickableOwner = CurrentPickable->GetOwner();

	if (!ensure(PickableOwner))
		return;

	CurrentPickable->SetActive(true, false);
	PickableOwner->DetachFromActor({FDetachmentTransformRules::KeepWorldTransform});
	CurrentPickable->GetOwner()->SetActorRotation(FRotator::ZeroRotator);

	if (auto* PickableMeshBase = PickableOwner->GetComponentByClass(UStaticMeshComponent::StaticClass()))
	{
		UStaticMeshComponent* PickableMesh = Cast<UStaticMeshComponent>(PickableMeshBase);
		PickableMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		if (bLaunch)
    {
			CurrentPickable->Throw(PickableAttachment->GetForwardVector() * LaunchSpeed);
		}
		else
		{
      PickableMesh->SetSimulatePhysics(true);
      PickableMesh->AddForce(PickableAttachment->GetForwardVector() * PickableMesh->GetMass() * DropForceScale);
      CurrentPickable->GetInteractable()->SetCanBeInteracted(true);
      CurrentPickable->GetInteractable()->SetIsCloseMaterialOverlay();
    }
	}

	OnDropPickable.Broadcast(CurrentPickable, bLaunch);
  CurrentPickable = nullptr;

  APestilenceCharacter* Character = Cast<APestilenceCharacter>(GetOwner());
  if (!Character) return;
  Character->GetInteractorComponent()->Enable(true);
}

// -----------------------------------------

void UInventoryComponent::ClearPickable()
{
	if (!CurrentPickable)
		return;
	
	CurrentPickable->GetOwner()->Destroy();
	CurrentPickable = nullptr;
}

// -----------------------------------------

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

// -----------------------------------------