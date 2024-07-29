// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleAbilityState.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "Pestilence/Components/Abilities/PestilenceAbility.h"
#include "Kismet/GameplayStatics.h"

void UIdleAbilityState::CreateState()
{
  Super::CreateState();

  StateType = EAbilityState::Idle;
}

void UIdleAbilityState::EnterState()
{
  Super::EnterState();
}

void UIdleAbilityState::UpdateState(float _DeltaTime)
{
  Super::UpdateState(_DeltaTime);
}

void UIdleAbilityState::ExitState()
{
  Super::ExitState();
}

void UIdleAbilityState::StartCast()
{
  Super::StartCast();
  const UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  if(CurrentAbility->GetCanReuse())
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetUseState());
}

void UIdleAbilityState::CancelCast()
{
  Super::CancelCast();
}

void UIdleAbilityState::StartPrepare()
{
  Super::StartPrepare();
  GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetPrepareState());
}

void UIdleAbilityState::CancelPrepare()
{
  Super::CancelPrepare();
}

bool UIdleAbilityState::CanSelectAbility()
{
  bool bCanSelectAbility = Super::CanSelectAbility();
  return bCanSelectAbility;
}