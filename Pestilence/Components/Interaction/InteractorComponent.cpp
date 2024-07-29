#include "InteractorComponent.h"
#include "InteractableComponent.h"
#include "Components/SphereComponent.h"
#include "Pestilence/Misc/PestilenceCollisionChannels.h"
#include "../Movement/PestilenceMovementComponent.h"

// -----------------------------------------

UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// -----------------------------------------
// al salir de un escondite me sale como 0 interactuables cerca aunque tenga un guardia al lado, tengo que mirarlo que hace que no se pueda interactuar hasta que te alejas de todos los interactuables
void UInteractorComponent::Initialize(USphereComponent* InteractSphere, USceneComponent* LookSource)
{
	Reset();
	
	InteractSphereComponent = InteractSphere;
	LookComponent = LookSource;
	
	if (!ensure(InteractSphereComponent) || !ensure(LookComponent))
		return;
	
	InteractSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractSphereComponent->SetCollisionResponseToChannel(ECC_INTERACTABLE, ECR_Overlap);
	
	InteractSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UInteractorComponent::OnShapeBeginOverlapCallback);
	InteractSphereComponent->OnComponentEndOverlap.AddDynamic(this, &UInteractorComponent::OnShapeEndOverlapCallback);
	
	InteractSphereRadius = InteractSphereComponent->GetUnscaledSphereRadius();

	SetActive(true);
	SearchForNearbyInteractables();
}

// -----------------------------------------

bool UInteractorComponent::TryInteract()
{
	if (!CurrentInteractable || !IsValid(CurrentInteractable) || !CurrentInteractable->IsValidLowLevel())
	{
		if (OnTriedToInteract.IsBound())
			OnTriedToInteract.Broadcast(false, false, nullptr);
		return false;
	}

	const bool bSuccess = CurrentInteractable->Interact(this);

	if (OnTriedToInteract.IsBound())
		OnTriedToInteract.Broadcast(true, bSuccess, CurrentInteractable);

	return true;
}

// -----------------------------------------

void UInteractorComponent::SearchForNearbyInteractables()
{
	if (!IsActive())
		return;

	const AActor* Owner = GetOwner();

	if (!Owner || !InteractSphereComponent)
		return;
	
	TArray<FHitResult> Results;
	const FVector Start =Owner->GetActorLocation();
	const FVector End = Start + FVector::ForwardVector * 0.001f;
	const FQuat Rot = Owner->GetActorRotation().Quaternion();
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractSphereComponent->GetUnscaledSphereRadius());
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(Owner);

	if (GetWorld()->SweepMultiByObjectType(Results, Start, End, Rot, ECC_INTERACTABLE, Sphere, CollisionParams))
	{
		NearbyInteractables = Results.Num();

		if (!IsComponentTickEnabled() && NearbyInteractables)
			SetComponentTickEnabled(true);
		else if (IsComponentTickEnabled() && !NearbyInteractables)
			SetComponentTickEnabled(false);

	}
	else
		NearbyInteractables = 0;
}

void UInteractorComponent::Enable(bool _Enable)
{
	if (InteractSphereComponent)
	{
		InteractSphereComponent->SetActive(_Enable);
		float NewRadius = _Enable ? InteractSphereRadius : 0.f;
		InteractSphereComponent->SetSphereRadius(NewRadius);
	}

  SetActive(_Enable);
	if (_Enable)
		SearchForNearbyInteractables();
	else
		Reset();

  SetComponentTickEnabled(_Enable && NearbyInteractables != 0);
}

// -----------------------------------------

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// -----------------------------------------

void UInteractorComponent::Activate(const bool bReset)
{
	Super::Activate(bReset);
	if (InteractSphereComponent)
		InteractSphereComponent->SetActive(true);
	
	SetComponentTickEnabled(NearbyInteractables != 0);
}

// -----------------------------------------

void UInteractorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	NearbyInteractables = 0;
	
	if (!ensure(InteractSphereComponent))
		return;

	InteractSphereComponent->SetActive(false);
	InteractSphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UInteractorComponent::OnShapeBeginOverlapCallback);
	InteractSphereComponent->OnComponentEndOverlap.RemoveDynamic(this, &UInteractorComponent::OnShapeEndOverlapCallback);
}

// -----------------------------------------

void UInteractorComponent::Deactivate()
{
	Super::Deactivate();
	if (InteractSphereComponent)
		InteractSphereComponent->SetActive(false);
	Reset();
	
	SetComponentTickEnabled(NearbyInteractables != 0);
}

// -----------------------------------------

void UInteractorComponent::Reset()
{
	if (CurrentInteractable && IsValid(CurrentInteractable) && CurrentInteractable->IsValidLowLevel())
	{
		CurrentInteractable->SetNoMaterialOverlay();
		CurrentInteractable->HoverEnd(this);
	}

	if (IsComponentTickEnabled())
		SetComponentTickEnabled(false);
	NearbyInteractables = 0;
	CurrentInteractable = nullptr;
  PreviousInteractable = nullptr;
  if (OnLookingInteractableChanged.IsBound())
    OnLookingInteractableChanged.Broadcast(PreviousInteractable, CurrentInteractable);
}

// -----------------------------------------

void UInteractorComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	SearchForLookingInteractable();
	PollEvents();
}

// -----------------------------------------

void UInteractorComponent::SearchForLookingInteractable()
{
	if (!LookComponent)
		return;

  CurrentInteractable = nullptr;

	FHitResult HitResult;
	
	const FVector ForwardVector = LookComponent->GetForwardVector();
	const FVector LookOrigin = LookComponent->GetComponentLocation();
	FVector LookDestination = ForwardVector * InteractionDistance + LookOrigin;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
	// If is hit
	if (GetWorld()->LineTraceSingleByChannel(HitResult, LookOrigin, LookDestination, ECC_INTERACTABLE, CollisionParams))
	{

		AActor* HitComponentOwner = HitResult.GetComponent()->GetOwner();
		UActorComponent* PossibleInteractable = HitComponentOwner->GetComponentByClass(UInteractableComponent::StaticClass());

		auto* InteractableHit = Cast<UInteractableComponent>(PossibleInteractable);

		if (!InteractableHit)
			return;

		FHitResult HitResultVisibility;

		// If is visible
		if (GetWorld()->LineTraceSingleByChannel(HitResultVisibility, LookOrigin, LookDestination, ECC_Camera, CollisionParams))
		{
			if (HitResultVisibility.GetComponent() != HitResult.GetComponent()) return;

			CurrentInteractable = InteractableHit;
		}
	}
}

void UInteractorComponent::PollEvents()
{
	if (CurrentInteractable != PreviousInteractable)
	{
		if (PreviousInteractable && IsValid(PreviousInteractable) && PreviousInteractable->IsValidLowLevel())
		{
      PreviousInteractable->HoverEnd(this);
      if (PreviousInteractable->GetCanBeInteracted())
        PreviousInteractable->SetIsCloseMaterialOverlay();
			else
        PreviousInteractable->SetNoMaterialOverlay();
		}
		if (CurrentInteractable && IsValid(CurrentInteractable) && CurrentInteractable->IsValidLowLevel())
		{
			CurrentInteractable->HoverBegin(this);
      if (CurrentInteractable->GetCanBeInteracted())
				CurrentInteractable->SetIsInteractableMaterialOverlay();
      else
				CurrentInteractable->SetNoMaterialOverlay();
		}

		if (OnLookingInteractableChanged.IsBound())
			OnLookingInteractableChanged.Broadcast(PreviousInteractable, CurrentInteractable);
		
		PreviousInteractable = CurrentInteractable;

	}
	
	if (CurrentInteractable)
	{
		CurrentInteractable->HoverStay(this);

		if (OnLookingInteractable.IsBound())
			OnLookingInteractable.Broadcast(CurrentInteractable);
	}
}

// -----------------------------------------

void UInteractorComponent::OnShapeBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                       const FHitResult& SweepResult)
{
	if (!IsActive()) 
		return;

	NearbyInteractables++;
	
	if (!IsComponentTickEnabled() && NearbyInteractables)
		SetComponentTickEnabled(true);

	UInteractableComponent* Interactable = Cast<UInteractableComponent>(OtherActor->GetComponentByClass(UInteractableComponent::StaticClass()));
  if (Interactable->GetCanBeInteracted() && !Interactable->IsIsInteractableMaterialOverlaySet())
		Interactable->SetIsCloseMaterialOverlay();
}

// -----------------------------------------

void UInteractorComponent::OnShapeEndOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsActive())
		return;

  UInteractableComponent* Interactable = Cast<UInteractableComponent>(OtherActor->GetComponentByClass(UInteractableComponent::StaticClass()));
	if(Interactable != CurrentInteractable && !Interactable->ContainsSameMesh(CurrentInteractable))
		Interactable->SetNoMaterialOverlay();

  NearbyInteractables = NearbyInteractables > 0 ? NearbyInteractables - 1 : 0;
  const UPestilenceMovementComponent* MovementComponent = Cast<UPestilenceMovementComponent>(GetOwner()->GetComponentByClass(UPestilenceMovementComponent::StaticClass()));

	if ((!NearbyInteractables && !MovementComponent) || (!NearbyInteractables && !MovementComponent->IsHidden()))
	{
    Reset();
	}
}

// -----------------------------------------