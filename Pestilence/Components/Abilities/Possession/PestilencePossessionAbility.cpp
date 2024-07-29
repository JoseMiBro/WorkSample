#include "Pestilence/Components/Abilities/Possession/PestilencePossessionAbility.h"

#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/Components/Abilities/PestilenceAbilityComponent.h"
#include "../../Possession/PestilenceVisionComponent.h"

/// -----------------------------------

void UPestilencePossessionAbility::Initialize_Implementation(UWorld* _World)
{
    Super::Initialize_Implementation(_World);

    SetAbilityType(EAbility::Possession);
}

/// -----------------------------------

void UPestilencePossessionAbility::StartPrepare_Implementation(bool& _OutResult)
{
  Super::StartPrepare_Implementation(_OutResult);
  PossessionComponent->StartVision();
}

/// -----------------------------------

void UPestilencePossessionAbility::EndPrepare_Implementation()
{
  Super::EndPrepare_Implementation();
  PossessionComponent->CancelVision();
}

/// -----------------------------------

void UPestilencePossessionAbility::StartCast_Implementation(bool& _OutResult)
{
  PossessionComponent->StartVision();
  PossessionComponent->StartPossession();

  _OutResult = GetOwner()->GetPestilenceVisionComponent()->GetLookingCharacter() != nullptr;
  Super::StartCast_Implementation(_OutResult);
}

/// -----------------------------------

void UPestilencePossessionAbility::CancelCast_Implementation()
{
  Super::CancelCast_Implementation();
  PossessionComponent->CancelPossession();
  PossessionComponent->CancelVision();

}

void UPestilencePossessionAbility::StartAbility_Implementation()
{
  Super::StartAbility_Implementation();
  PossessionComponent->CancelVision();
}

void UPestilencePossessionAbility::SetNewOwner(APestilenceCharacter* _Owner)
{
  Super::SetNewOwner(_Owner);

  PossessionComponent = _Owner->GetPestilencePossessionComponent();
  PossessionComponent->CancelVision();
}
