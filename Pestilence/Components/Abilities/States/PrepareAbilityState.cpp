// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepareAbilityState.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "Pestilence/Components/Abilities/PestilenceAbility.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "../../Perception/SuspiciousActionSenderComponent.h"
#include "../../../Enums/SuspiciousActionType.h"

void UPrepareAbilityState::CreateState()
{
  Super::CreateState();

  StateType = EAbilityState::Prepare;
}

void UPrepareAbilityState::EnterState()
{
  Super::EnterState();
  
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  GetAbilityComponent()->BroadcastPrepareStarted();
  bool OutResult = true;
  CurrentAbility->StartPrepare(OutResult);
  if (!OutResult)
    CancelPrepare();

  float CurrentIntegrity = GetAbilityComponent()->GetCurrentIntegrity();
  if (CurrentIntegrity < CurrentAbility->IntegrityCost)
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetPrepareNoIntegrityState());

}

void UPrepareAbilityState::UpdateState(float _DeltaTime)
{
  Super::UpdateState(_DeltaTime);

  float CurrentIntegrity = GetAbilityComponent()->GetCurrentIntegrity();
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();

  if (CurrentIntegrity < CurrentAbility->IntegrityCost)
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetPrepareNoIntegrityState());
  else
  {
    PrepareSuspiciousTimer += _DeltaTime;
    if (PrepareSuspiciousTimer >= GetAbilityComponent()->GetPrepareSuspiciousTime())
    {
      PrepareSuspiciousTimer -= GetAbilityComponent()->GetPrepareSuspiciousTime();

      if (CurrentAbility->GetAbilityType() != EAbility::Pickable)
        GetAbilityComponent()->GetPestilenceOwner()->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(GetAbilityComponent()->GetOwner(), ESuspiciousActionType::PrepareAbility);
    }

    bool OutResult = true;
    CurrentAbility->TickPrepare(_DeltaTime, OutResult);
    if (!OutResult)
      CancelPrepare();
  }
}

void UPrepareAbilityState::ExitState()
{
  Super::ExitState();

  GetAbilityComponent()->BroadcastPrepareEnded();
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  CurrentAbility->EndPrepare();
}

void UPrepareAbilityState::StartCast()
{
  Super::StartCast();
  const UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  if(CurrentAbility->GetCanReuse())
  {
    GetAbilityComponent()->BroadcastPrepareEnded();
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetUseState());
  }
  else
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetCastState());
}

void UPrepareAbilityState::CancelCast()
{
  Super::CancelCast();
}

void UPrepareAbilityState::StartPrepare()
{
  Super::StartPrepare();
  GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetPrepareState());
}

void UPrepareAbilityState::CancelPrepare()
{
  Super::CancelPrepare();
  GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetIdleState());
}

bool UPrepareAbilityState::CanSelectAbility()
{
  bool bCanSelectAbility = Super::CanSelectAbility();
  return bCanSelectAbility;
}