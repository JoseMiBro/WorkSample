// Fill out your copyright notice in the Description page of Project Settings.


#include "PrepareNoIntegrityAbilityState.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "Pestilence/Components/Abilities/PestilenceAbility.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "../../Perception/SuspiciousActionSenderComponent.h"
#include "../../../Enums/SuspiciousActionType.h"

void UPrepareNoIntegrityAbilityState::CreateState()
{
  Super::CreateState();

  StateType = EAbilityState::PrepareNoIntegrity;
}

void UPrepareNoIntegrityAbilityState::EnterState()
{
  Super::EnterState();

  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();

  bool OutResult = true;
  CurrentAbility->StartPrepare(OutResult);
  if (!OutResult)
    CancelPrepare();
  else
    GetAbilityComponent()->BroadcastPrepareNoIntegrityStarted();
}

void UPrepareNoIntegrityAbilityState::UpdateState(float _DeltaTime)
{
  Super::UpdateState(_DeltaTime);

  float CurrentIntegrity = GetAbilityComponent()->GetCurrentIntegrity();
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  if (CurrentIntegrity >= CurrentAbility->IntegrityCost)
    GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetPrepareState());
}

void UPrepareNoIntegrityAbilityState::ExitState()
{
  Super::ExitState();

  GetAbilityComponent()->BroadcastPrepareNoIntegrityEnded();
  UPestilenceAbility* CurrentAbility = GetAbilityComponent()->GetCurrentAbility();
  CurrentAbility->EndPrepare();
}

void UPrepareNoIntegrityAbilityState::StartCast()
{
  Super::StartCast();
  
  UGameplayStatics::PlaySound2D(this, GetAbilityComponent()->NotEnoughIntegritySound);
  GetAbilityComponent()->BroadcastNotEnoughIntegrity();
}

void UPrepareNoIntegrityAbilityState::CancelCast()
{
  Super::CancelCast();
}

void UPrepareNoIntegrityAbilityState::StartPrepare()
{
  Super::StartPrepare();
}

void UPrepareNoIntegrityAbilityState::CancelPrepare()
{
  Super::CancelPrepare();
  GetAbilityComponent()->ChangeState(GetAbilityComponent()->GetIdleState());
}

bool UPrepareNoIntegrityAbilityState::CanSelectAbility()
{
  bool bCanSelectAbility = Super::CanSelectAbility();
  return bCanSelectAbility;
}