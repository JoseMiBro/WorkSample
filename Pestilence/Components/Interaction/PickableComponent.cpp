#include "PickableComponent.h"
#include "InteractableComponent.h"
#include "InteractorComponent.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Perception/AISense_Hearing.h>
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ShapeComponent.h"
#include "Pestilence/Components/Perception/PestilenceHearingComponent.h"

UPickableComponent::UPickableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPickableComponent::SetActive(const bool bNewActive, const bool bReset)
{
	Super::SetActive(bNewActive, bReset);
	Interactable->SetActive(bNewActive, bReset);
}

UInteractableComponent* UPickableComponent::GetInteractable() const
{
  return Interactable;
}

void UPickableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bThrowed) return; 

	FQuat DeltaRotation = FQuat::MakeFromEuler(RotationSpeed * DeltaTime);
	MeshComponent->AddLocalRotation(DeltaRotation);
}

void UPickableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UActorComponent* PossibleInteractable = GetOwner()->GetComponentByClass(UInteractableComponent::StaticClass());

	if (!ensure(PossibleInteractable))
		return;
	
	const AActor* Owner = GetOwner();

	if (UActorComponent* BaseMesh = Owner->GetComponentByClass(UStaticMeshComponent::StaticClass()))
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(BaseMesh);
		MeshComponent = Mesh;
		for (auto& Channel : IgnoreChannels)
			Mesh->SetCollisionResponseToChannel(Channel, ECR_Ignore);
	}
	
	Interactable = Cast<UInteractableComponent>(PossibleInteractable);
	Interactable->InteractableType = EInteractableType::Pickable;
	Interactable->OnInteracted.AddDynamic(this, &UPickableComponent::OnInteracted);

	if (auto* PickableMeshBase = GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()))
	{
		UStaticMeshComponent* PickableMesh = Cast<UStaticMeshComponent>(PickableMeshBase);
		PickableMesh->OnComponentHit.AddDynamic(this, &UPickableComponent::OnComponentHitCallback);
	}

	SetActive(true, false);
}

void UPickableComponent::Initialize(UProjectileMovementComponent* ProjectileComponent, UShapeComponent* PhysicsShapeComponent)
{
	ProjectileMovementComponent = ProjectileComponent;
	ProjectileMovementComponent->SetComponentTickEnabled(false);

	PhysicsShape = PhysicsShapeComponent;
}

void UPickableComponent::Throw(const FVector& ThrowSpeed)
{
	if (ProjectileMovementComponent)
  {
		bThrowed = true;
		float RandomNum = FMath::RandRange(0.f, 1.f);
		RotationSpeed = FMath::Lerp(MinRotationSpeed, MaxRotationSpeed, RandomNum);

    ProjectileMovementComponent->SetActive(true, true);
    ProjectileMovementComponent->SetComponentTickEnabled(true);
    ProjectileMovementComponent->SetVelocityInLocalSpace(ThrowSpeed);
    PhysicsShape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UPickableComponent::OnInteracted(const bool bResult, UInteractorComponent* Interactor)
{
	if (!bResult || !IsActive())
		return;
	
	const AActor* InteractorOwner = Interactor->GetOwner();

	if (!ensure(InteractorOwner))
		return;
	
	if (auto* InventoryBase = InteractorOwner->GetComponentByClass(UInventoryComponent::StaticClass()))
	{
		UInventoryComponent* Inventory = Cast<UInventoryComponent>(InventoryBase);
		Inventory->TakePickable(this);
  }

}

void UPickableComponent::OnComponentHitCallback(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
  if (bIsBreakable && !bHasBeenHit && GetOwner()->GetVelocity().SquaredLength() >= BreakSpeed * BreakSpeed)
	{
		bHasBeenHit = true;
		
		if (BreakSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetOwner()->GetActorLocation());
			UAISense_Hearing::ReportNoiseEvent(
				GetWorld(),
				GetOwner()->GetActorLocation(),
				1,
				GetOwner(),
				BreakNoiseRangeAI,
				UPestilenceHearingComponent::SoundTypeToString(SoundType::BottleBraking)
				);
		}
		
		GetOwner()->SetActorHiddenInGame(true);
		GetOwner()->SetActorEnableCollision(false);

		OnHitCallbackCompleted.Broadcast();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPickableComponent::DestroyOnbreak, 5.0f);
	}
	else if (bThrowed && ProjectileMovementComponent)
	{
    bThrowed = false;
    ProjectileMovementComponent->SetActive(false);
    ProjectileMovementComponent->SetComponentTickEnabled(false);
    PhysicsShape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
    Interactable->SetCanBeInteracted(true);
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->AddForce(-MeshComponent->GetForwardVector() * MeshComponent->GetMass() * GetOwner()->GetVelocity().SquaredLength());
    
	}	
}

void UPickableComponent::DestroyOnbreak()
{
	GetOwner()->Destroy();
}
