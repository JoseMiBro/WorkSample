#include "InteractableComponent.h"
#include "Components/ShapeComponent.h"
#include "Pestilence/Misc/PestilenceCollisionChannels.h"

// -----------------------------------------

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// -----------------------------------------

void UInteractableComponent::Initialize(UShapeComponent* InteractShape)
{
	InteractShapeComponent = InteractShape;
	
	if (!InteractShapeComponent)
		return;
	
	InteractShapeComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	InteractShapeComponent->SetCollisionResponseToChannel(ECC_INTERACTABLE, ECR_Block);
	
	//InteractShapeComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	InteractShapeComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	
	InteractShapeComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	InteractShapeComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	InteractShapeComponent->SetCollisionResponseToChannel(ECC_PROJECTILE, ECR_Overlap);
	InteractShapeComponent->SetCollisionObjectType(ECC_INTERACTABLE);

	SetActive(true, false);
}

// -----------------------------------------

void UInteractableComponent::SetActive(const bool bNewActive, const bool bReset)
{
	Super::SetActive(bNewActive, bReset);

	if (!InteractShapeComponent)
		return;
	
	InteractShapeComponent->SetCollisionEnabled(bNewActive ?
		ECollisionEnabled::QueryOnly :
		ECollisionEnabled::NoCollision);
}

// -----------------------------------------

void UInteractableComponent::SetIsCloseMaterialOverlay()
{
	if (MeshComponents.Num() == 0) return;

  for (int i = 0; i < MeshComponents.Num(); ++i)
  {
		MeshComponents[i]->SetOverlayMaterial(IsCloseMaterial);
  }
}

// -----------------------------------------

void UInteractableComponent::SetIsInteractableMaterialOverlay()
{
  if (MeshComponents.Num() == 0) return;

  for (int i = 0; i < MeshComponents.Num(); ++i)
  {
    MeshComponents[i]->SetOverlayMaterial(IsInteractableMaterial);
  }
}

// -----------------------------------------

void UInteractableComponent::SetNoMaterialOverlay()
{
  if (MeshComponents.Num() == 0) return;

  for (int i = 0; i < MeshComponents.Num(); ++i)
  {
    MeshComponents[i]->SetOverlayMaterial(nullptr);
  }
}

// -----------------------------------------

bool UInteractableComponent::IsCloseMaterialOverlaySet()
{
  if (MeshComponents.Num() == 0) return false;

	return MeshComponents[0]->GetOverlayMaterial() == IsCloseMaterial;
}

// -----------------------------------------

bool UInteractableComponent::IsIsInteractableMaterialOverlaySet()
{
  if (MeshComponents.Num() == 0) return false;

  return MeshComponents[0]->GetOverlayMaterial() == IsInteractableMaterial;
}

// -----------------------------------------

bool UInteractableComponent::IsNoMaterialOverlaySet()
{
  if (MeshComponents.Num() == 0) return false;

  return MeshComponents[0]->GetOverlayMaterial() == nullptr;
}

// -----------------------------------------

bool UInteractableComponent::ContainsSameMesh(UInteractableComponent* OtherInteractable)
{
	if (!OtherInteractable) return false;

	bool bFound = false;
	int i = 0;
	while(!bFound && i < MeshComponents.Num())
	{
		bFound = OtherInteractable->MeshComponents.Contains(MeshComponents[i]);
		++i;
	}
	return bFound;
}

// -----------------------------------------

void UInteractableComponent::SetCanBeInteracted(bool _bCanBeInteracted)
{
  //InteractShapeComponent->SetCollisionResponseToChannel(ECC_Visibility, _bCanBeInteracted ? ECR_Block : ECR_Ignore);
  InteractShapeComponent->SetCollisionResponseToChannel(ECC_Camera, _bCanBeInteracted ? ECR_Block : ECR_Ignore);
  InteractShapeComponent->SetCollisionResponseToChannel(ECC_INTERACTABLE, _bCanBeInteracted ? ECR_Block : ECR_Ignore);
	bCanBeInteracted = _bCanBeInteracted;
}

// -----------------------------------------

bool UInteractableComponent::GetCanBeInteracted() const
{
	return bCanBeInteracted;
}

// -----------------------------------------

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetComponents<UMeshComponent>(MeshComponents);
	for(int i = 0; i < ActorsToHighlight.Num(); ++i)
  {
		if(!ActorsToHighlight[i]) continue;

		ActorsToHighlight[i]->ForEachComponent<UStaticMeshComponent>(true, [&](UActorComponent* InComp)
    {
      MeshComponents.Add(Cast<UStaticMeshComponent>(InComp));
    });
	}
}

// -----------------------------------------

bool UInteractableComponent::Interact(UInteractorComponent* Sender)
{
	if (!IsActive() || !bCanBeInteracted)
		return false;
	
	bool bSuccess = true;
	
	if (!bEvaluateFromBlueprint && InteractDelegate.IsBound())
		bSuccess = InteractDelegate.Execute(Sender);

	if (bEvaluateFromBlueprint)
		bSuccess = EvaluateInteraction(Sender);
		
	OnInteracted.Broadcast(bSuccess, Sender);
	return bSuccess;
}

// -----------------------------------------

void UInteractableComponent::HoverBegin(UInteractorComponent* Sender)
{
	if (!IsActive() || !bCanBeInteracted)
		return;
	
	if (OnHoverBegin.IsBound())
		OnHoverBegin.Broadcast(Sender);
}

// -----------------------------------------

void UInteractableComponent::HoverStay(UInteractorComponent* Sender)
{
	if (!IsActive() || !bCanBeInteracted)
		return;
	
	if (OnHoverStay.IsBound())
		OnHoverStay.Broadcast(Sender);
}

// -----------------------------------------

void UInteractableComponent::HoverEnd(UInteractorComponent* Sender)
{
	if (!IsActive() || !bCanBeInteracted)
		return;
	
	if (OnHoverEnd.IsBound())
		OnHoverEnd.Broadcast(Sender);
}

// -----------------------------------------