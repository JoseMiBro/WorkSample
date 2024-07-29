// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbilityState.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "../../Interaction/InventoryComponent.h"

TObjectPtr<UPestilenceAbilityComponent> UBaseAbilityState::GetAbilityComponent() { return AbilityComponent; }
void UBaseAbilityState::CreateState()
{
}

void UBaseAbilityState::EnterState()
{
	bIsActive = true;
}

void UBaseAbilityState::UpdateState(float _DeltaTime)
{
}

void UBaseAbilityState::ExitState()
{
	bIsActive = false;
}

void UBaseAbilityState::StartCast()
{
}

void UBaseAbilityState::CancelCast()
{
}

void UBaseAbilityState::StartPrepare()
{
}

void UBaseAbilityState::CancelPrepare()
{
}

bool UBaseAbilityState::CanSelectAbility()
{
	return !GetAbilityComponent()->GetPestilenceOwner()->GetInventoryComponent()->HasPickable();
}

void UBaseAbilityState::SetAbilityComponent(const TObjectPtr<UPestilenceAbilityComponent>& _AbilityComponent)
{
	AbilityComponent = _AbilityComponent;
	CreateState();
}

EAbilityState UBaseAbilityState::GetStateType() const
{
	return StateType;
}
