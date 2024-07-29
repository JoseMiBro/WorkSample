// Fill out your copyright notice in the Description page of Project Settings.


#include "CastAbilityState.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "Pestilence/Components/Abilities/PestilenceAbility.h"

void UCastAbilityState::CreateState()
{
  Super::CreateState();

  StateType = EAbilityState::Cast;
}

void UCastAbilityState::EnterState()
{
  Super::EnterState();

  GetAbilityComponent()->BroadcastCastStarted();
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility(); 

  bool OutResult = true;
  CurrentAbility->StartCast(OutResult);
  if(OutResult)
  {
    if (CurrentAbility->CastTime > 0)
    {
      CastTimer = 0.f;
    }
    else
    {
      EndCast();
    }
  }
  else
  {
    CancelCast();
  }

}

void UCastAbilityState::UpdateState(float _DeltaTime)
{
  Super::UpdateState(_DeltaTime);

  CastTimer += _DeltaTime;
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  bool OutResult = true; 
  CurrentAbility->TickCast(_DeltaTime, OutResult);
  if(OutResult)
  {
    if (CastTimer >= CurrentAbility->CastTime)
    {
      EndCast();
    }
  }
  else
  {
    CancelCast();
  }
  
}

void UCastAbilityState::ExitState()
{
  Super::ExitState();
}

void UCastAbilityState::StartCast()
{
  Super::StartCast();
}

void UCastAbilityState::CancelCast()
{
  Super::CancelCast();
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  CurrentAbility->CancelCast();

  if(GetAbilityComponent()->GetIsTryPrepare())
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetPrepareState());
  else  
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetIdleState());
}

void UCastAbilityState::StartPrepare()
{
  Super::StartPrepare();
  GetAbilityComponent()->BroadcastPrepareStarted();
}

void UCastAbilityState::CancelPrepare()
{
  Super::CancelPrepare();
  GetAbilityComponent()->BroadcastPrepareEnded();
}

bool UCastAbilityState::CanSelectAbility()
{
  bool bCanSelectAbility = Super::CanSelectAbility();
  
  return bCanSelectAbility;
}

void UCastAbilityState::EndCast()
{
  bool OutResult = true;
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  CurrentAbility->EndCast(OutResult);
  if (OutResult)
  {
    float CurrentIntegrity = GetAbilityComponent()->GetCurrentIntegrity();
    CurrentIntegrity -= CurrentAbility->IntegrityCost;
    CurrentIntegrity = CurrentIntegrity < 0 ? 0 : CurrentIntegrity;
    GetAbilityComponent()->SetCurrentIntegrity(CurrentIntegrity);

    GetAbilityComponent()->BroadcastPrepareEnded();
    GetAbilityComponent()->BroadcastCastEnded();

    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetUseState());
  }
  else
  {
    CancelCast();
  }
}