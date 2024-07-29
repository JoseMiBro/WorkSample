#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "FreeState.h"
#include "PossessedState.h"
#include "PossessionState.h"
#include "RagdollState.h"
#include "TransitionState.h"
#include "../../Characters/PestilenceCharacter.h"

UPestilencePossessionComponent::UPestilencePossessionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPestilencePossessionComponent::SetPossessed(const bool bShouldBePossessed)
{
	bIsPossessed = bShouldBePossessed;
}

void UPestilencePossessionComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<APestilenceCharacter>(GetOwner());

	PossessedState = NewObject<UPossessedState>(this);
	PossessedState->SetPossessionComponent(this);

	TransitionState = NewObject<UTransitionState>(this);
	TransitionState->SetPossessionComponent(this);
	
	FreeState = NewObject<UFreeState>(this);
	FreeState->SetPossessionComponent(this);

	RagdollState = NewObject<URagdollState>(this);
	RagdollState->SetPossessionComponent(this);
	
	if (bStartPossessed) SwitchState(PossessedState);
	else SwitchState(FreeState);
}

void UPestilencePossessionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!CurrentState) return;
	CurrentState->UpdateState(DeltaTime);
}

float UPestilencePossessionComponent::GetCurrentIntegrity() const
{
	return PossessedState->GetCurrentIntegrity();
}

void UPestilencePossessionComponent::SetCurrentIntegrity(const float _CurrentIntegrity)
{
  IntegrityChangedDelegate.Broadcast(PossessedState->GetCurrentIntegrity(), _CurrentIntegrity);
	return PossessedState->SetCurrentIntegrity(_CurrentIntegrity);
}

void UPestilencePossessionComponent::SwitchState(UPossessionState* NewState)
{
	if (CurrentState)
		CurrentState->ExitState();
	CurrentState = NewState;
	CurrentState->EnterState();
}

void UPestilencePossessionComponent::StartPossession() { PossessionInputEvent.Broadcast(true); }
void UPestilencePossessionComponent::CancelPossession() { PossessionInputEvent.Broadcast(false); }
void UPestilencePossessionComponent::StartVision() { VisionInputEvent.Broadcast(true); }
void UPestilencePossessionComponent::CancelVision() { VisionInputEvent.Broadcast(false); }