#include "Pestilence/Components/AI/PestilenceAIComponent.h"

// Sets default values for this component's properties
UPestilenceAIComponent::UPestilenceAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPestilenceAIComponent::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent = Cast<USphereComponent>(GetOwner()->GetComponentByClass(USphereComponent::StaticClass()));
	
}
