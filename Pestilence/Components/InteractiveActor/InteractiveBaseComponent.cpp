#include "Pestilence/Components/InteractiveActor/InteractiveBaseComponent.h"

UInteractiveBaseComponent::UInteractiveBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Owner = GetOwner<AActor>();

}

void UInteractiveBaseComponent::DefaultInteraction(APestilenceCharacter* CharActor)
{
	if (Owner && bIsInteractuable)
	{
	}
}


void UInteractiveBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UInteractiveBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

