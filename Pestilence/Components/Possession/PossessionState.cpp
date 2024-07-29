#include "Pestilence/Components/Possession/PossessionState.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"

void UPossessionState::CreateState()
{
}

void UPossessionState::EnterState()
{
	bIsActive = true;
}

void UPossessionState::UpdateState(float DeltaTime)
{
}

void UPossessionState::ExitState()
{
	bIsActive = false;
}

void UPossessionState::SetPossessionComponent(const TObjectPtr<UPestilencePossessionComponent>& Possession)
{
	PossessionComponent = Possession;
	CreateState();
}