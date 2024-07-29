// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceInfectAbility.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "../../../Components/Possession/PestilencePossessionComponent.h"
#include "../../../Components/Possession/PestilenceVisionComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../AI/Interface/PestilenceAIInterface.h"
#include "../../../AI/PestilenceAIController.h"
#include "../../Movement/PestilenceMovementComponent.h"
#include "InfectionDragonflies.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/Behaviors/blackboard_keys.h"
#include "Pestilence/Components/Perception/PestilenceHearingComponent.h"


void UPestilenceInfectAbility::Initialize_Implementation(UWorld* _World)
{
  Super::Initialize_Implementation(_World);

  SetAbilityType(EAbility::Infection);
  InfectionDragonflies = _World->SpawnActor<AInfectionDragonflies>(InfectionDragonfliesClass);

  InfectionDragonflies->OnTargetReached.AddDynamic(this, &UPestilenceInfectAbility::Infect);
  InfectionDragonflies->OnTargetReached.AddDynamic(this, &UPestilenceInfectAbility::GenerateInfectedSound);
}

void UPestilenceInfectAbility::StartPrepare_Implementation(bool& _OutResult)
{
  _OutResult = !InfectionDragonflies->IsFlying();
  Super::StartPrepare_Implementation(_OutResult);

  if (!_OutResult) return;
  GetOwner()->GetPestilenceVisionComponent()->SetActive(true);
}

void UPestilenceInfectAbility::EndPrepare_Implementation()
{
  Super::EndPrepare_Implementation();
  GetOwner()->GetPestilenceVisionComponent()->SetActive(false);
}

void UPestilenceInfectAbility::StartCast_Implementation(bool& _OutResult)
{
  GetOwner()->GetPestilenceVisionComponent()->SetActive(true);
  CurrentTarget = GetOwner()->GetPestilenceVisionComponent()->GetLookingCharacter();
  IPestilenceAIInterface* PestilenceAIInterface = Cast<IPestilenceAIInterface>(CurrentTarget);

  if (CurrentTarget)
    CurrentTarget->SetOverlayMaterialUsing();

  if (bDisableInputOnInfect)
  {
    GetOwner()->GetPestilenceMovementComponent()->EnableMovement(false);
    GetOwner()->GetPestilenceMovementComponent()->EnableLook(false);
  }

  _OutResult = CurrentTarget != nullptr && 
               PestilenceAIInterface && PestilenceAIInterface->GetTypeCharacter() == EPestilenceTypeCharacter::Pawn;


  Super::StartCast_Implementation(_OutResult);
}

void UPestilenceInfectAbility::TickCast_Implementation(float _DeltaTime, bool& _OutResult)
{
  Super::TickCast_Implementation(_DeltaTime, _OutResult);

  if (!CurrentTarget) return;

  CurrentTarget->SetOverlayMaterialUsing();

  FVector Direction = CurrentTarget->GetActorLocation() - GetOwner()->GetFirstPersonCameraComponent()->GetComponentLocation();
  FRotator NewRot = Direction.Rotation();
  if (GetOwner()->GetController())
    GetOwner()->GetController()->SetControlRotation(NewRot);
  else
    GetOwner()->SetActorRotation(NewRot);
}

void UPestilenceInfectAbility::EndCast_Implementation(bool& _OutResult)
{
  Super::EndCast_Implementation(_OutResult);

  APestilenceCharacter* NewTarget = GetOwner()->GetPestilenceVisionComponent()->GetLookingCharacter();

  if (CurrentTarget)
    CurrentTarget->SetOverlayMaterialNone();

  if (bDisableInputOnInfect)
  {
    GetOwner()->GetPestilenceMovementComponent()->EnableMovement(true);
    GetOwner()->GetPestilenceMovementComponent()->EnableLook(true);
  }

  GetOwner()->GetPestilenceVisionComponent()->SetActive(false);
  _OutResult = _OutResult && NewTarget == CurrentTarget;
}

void UPestilenceInfectAbility::CancelCast_Implementation()
{
  Super::CancelCast_Implementation();
  GetOwner()->GetPestilenceVisionComponent()->SetActive(false);

  if (CurrentTarget)
    CurrentTarget->SetOverlayMaterialNone();

  if (bDisableInputOnInfect)
  {
    GetOwner()->GetPestilenceMovementComponent()->EnableMovement(true);
    GetOwner()->GetPestilenceMovementComponent()->EnableLook(true);
  }
}

void UPestilenceInfectAbility::StartAbility_Implementation()
{
  Super::StartAbility_Implementation();
  GetOwner()->GetPestilenceVisionComponent()->SetActive(false);
}

void UPestilenceInfectAbility::EndAbility_Implementation()
{
  Super::EndAbility_Implementation();

}

void UPestilenceInfectAbility::UseAbilityNotify()
{
  InfectionDragonflies->StartFlying(GetOwner()->GetAbilityAttachment()->GetComponentLocation(), CurrentTarget);
}

void UPestilenceInfectAbility::GenerateInfectedSound()
{
  UAISense_Hearing::ReportNoiseEvent(
    GetWorld(),
    CurrentTarget->GetActorLocation(),
    1,
    GetOwner(),
    InfectedSoundRange,
    UPestilenceHearingComponent::SoundTypeToString(SoundType::Infection)
    );
  
  UGameplayStatics::PlaySoundAtLocation(
    GetWorld(),
    InfectedSound,CurrentTarget->GetActorLocation(),
    CurrentTarget->GetActorRotation(),
    InfectedSoundVolumeMultiplier
    );
}

void UPestilenceInfectAbility::Infect()
{
  CurrentTarget->GetPestilencePossessionComponent()->SetCurrentIntegrity(InfectIntegrity);
  CurrentTarget->StartImpactParticles();
  APestilenceAIController* AIController = Cast<APestilenceAIController>(CurrentTarget->GetController());
  if (!IsValid(AIController)) return;
  AIController->SetCurrentState(EPestilenceStates::Infected);
}
