// Fill out your copyright notice in the Description page of Project Settings.


#include "UseAbilityState.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "Pestilence/Components/Abilities/PestilenceAbility.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "../../Interaction/InteractorComponent.h"

void UUseAbilityState::CreateState()
{
  Super::CreateState(); 

  StateType = EAbilityState::Use;
}

void UUseAbilityState::EnterState()
{
  Super::EnterState();
  GetAbilityComponent()->BroadcastAbilityStarted();
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  CurrentAbility->StartAbility();
  GetAbilityComponent()->GetPestilenceOwner()->GetInteractorComponent()->Enable(false);
  UseTime = CurrentAbility->GetCanReuse() ? CurrentAbility->ReuseAbilityDuration : CurrentAbility->AbilityDuration;
  if (UseTime > 0)
  {
    UseTimer = 0.f;
  }
  else
  {
    EndUse();
  }
}

void UUseAbilityState::UpdateState(float _DeltaTime)
{
  Super::UpdateState(_DeltaTime);

  UseTimer += _DeltaTime;
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  CurrentAbility->TickAbility(_DeltaTime);

  if (UseTimer >= UseTime)
  {
    EndUse();
  }
}

void UUseAbilityState::ExitState()
{
  Super::ExitState();
  GetAbilityComponent()->GetPestilenceOwner()->GetInteractorComponent()->Enable(true);
}

void UUseAbilityState::StartCast()
{
  Super::StartCast();
}

void UUseAbilityState::CancelCast()
{
  Super::CancelCast();
}

void UUseAbilityState::StartPrepare()
{
  Super::StartPrepare();
}

void UUseAbilityState::CancelPrepare()
{
  Super::CancelPrepare();
}

bool UUseAbilityState::CanSelectAbility()
{
  Super::CanSelectAbility();
  return false;
}

void UUseAbilityState::EndUse()
{
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();

  GetAbilityComponent()->BroadcastAbilityEnded();
  CurrentAbility->EndAbility();

  if (GetAbilityComponent()->GetIsTryPrepare())
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetPrepareState());
  else
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetIdleState());
}
