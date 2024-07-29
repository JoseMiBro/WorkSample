#include "PickableReceiverComponent.h"
#include "InteractableComponent.h"
#include "InteractorComponent.h"
#include "InventoryComponent.h"
#include "PickableComponent.h"

UPickableReceiverComponent::UPickableReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPickableReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

	UActorComponent* PossibleInteractable = GetOwner()->GetComponentByClass(UInteractableComponent::StaticClass());

	if (!ensure(PossibleInteractable))
		return;
	
	Interactable = Cast<UInteractableComponent>(PossibleInteractable);
	Interactable->OnInteracted.AddDynamic(this, &UPickableReceiverComponent::OnInteracted);
	Interactable = Cast<UInteractableComponent>(PossibleInteractable);
}

void UPickableReceiverComponent::OnInteracted(bool bResult, UInteractorComponent* Interactor)
{
	if (!bResult)
	{
		OnPickableReceived.Broadcast("None", false);
		return;
	}

	const AActor* InteractorOwner = Interactor->GetOwner();

	if (!ensure(InteractorOwner))
	{
		OnPickableReceived.Broadcast("None", false);
		return;
	}

	UActorComponent* InventoryBase = InteractorOwner->GetComponentByClass(UInventoryComponent::StaticClass());

	if (!InventoryBase)
	{
		OnPickableReceived.Broadcast("None", false);
		return;
	}
	
	UInventoryComponent* Inventory = Cast<UInventoryComponent>(InventoryBase);

	if (!Inventory->HasPickable())
	{
		OnPickableReceived.Broadcast("None", false);
		return;
	}

	if (!Inventory->GetCurrentPickable()->PickableType.IsEqual(PickableType))
	{
		OnPickableReceived.Broadcast(PickableType, false);
		return;
	}

	OnPickableReceived.Broadcast(PickableType, true);
	Inventory->ClearPickable();
}