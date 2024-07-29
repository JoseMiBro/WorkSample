// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceCharacterAnimInstance.h"
#include "../../Characters/PestilenceCharacter.h"
#include "../../Components/Movement/PestilenceMovementComponent.h"
#include "../../AI/Interface/PestilenceAIInterface.h"
#include "../../AI/PestilenceAIController.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "../../AI/Behaviors/blackboard_keys.h"
#include "../../Components/Possession/PestilencePossessionComponent.h"
#include "../../Components/Possession/ArchetypeComponent.h"

void UPestilenceCharacterAnimInstance::NativeInitializeAnimation()
{
  Character = Cast<APestilenceCharacter>(GetOwningActor());
  if (!Character) return;
  MovementComponent = Character->GetPestilenceMovementComponent(); 
}

void UPestilenceCharacterAnimInstance::NativeBeginPlay()
{
  IPestilenceAIInterface* PestilenceAIInterface = Cast<IPestilenceAIInterface>(Character);

  CharacterType = PestilenceAIInterface->GetTypeCharacter();
  Gender = Character->GetArchetypeComponent()->GetPestilenceGender();

  if (!Character->OnKillPlayer.IsAlreadyBound(this, &UPestilenceCharacterAnimInstance::OnKillPlayer))
    Character->OnKillPlayer.AddDynamic(this, &UPestilenceCharacterAnimInstance::OnKillPlayer);
  if (!MovementComponent->OnClimbing.IsAlreadyBound(this, &UPestilenceCharacterAnimInstance::OnClimbing))
    MovementComponent->OnClimbing.AddDynamic(this, &UPestilenceCharacterAnimInstance::OnClimbing);
  if (!MovementComponent->OnUsingDoor.IsAlreadyBound(this, &UPestilenceCharacterAnimInstance::OnUsingDoor))
    MovementComponent->OnUsingDoor.AddDynamic(this, &UPestilenceCharacterAnimInstance::OnUsingDoor);
}

void UPestilenceCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
  if (!IsValid(Character) || !IsValid(MovementComponent)) return;
  Velocity = MovementComponent->Velocity;
  GroundSpeed = Velocity.Length();

  bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector(0.f, 0.f, 0.f);
  
  bIsFalling = MovementComponent->IsFalling();

  if (Character->GetPestilencePossessionComponent()->IsPossessed() || !IsValid(Character->GetControllerAI())) return;

  State = Character->GetControllerAI()->GetCurrentState();
  UBlackboardComponent* Blackboard = Character->GetControllerAI()->GetBlackboard();
  if (!IsValid(Blackboard)) return;

  bStartAssassinate = Blackboard->GetValueAsBool(Keys::StartAssassinate);
  bSomeoneInfected = Blackboard->GetValueAsBool(Keys::SomeOneInfected);
  bArmed = Blackboard->GetValueAsBool(Keys::Armed);
}

void UPestilenceCharacterAnimInstance::BeginDestroy()
{
  if(Character && MovementComponent)
  {
    if(Character->OnKillPlayer.IsAlreadyBound(this, &UPestilenceCharacterAnimInstance::OnKillPlayer))
      Character->OnKillPlayer.RemoveDynamic(this, &UPestilenceCharacterAnimInstance::OnKillPlayer);
    if (MovementComponent->OnClimbing.IsAlreadyBound(this, &UPestilenceCharacterAnimInstance::OnClimbing))
      MovementComponent->OnClimbing.RemoveDynamic(this, &UPestilenceCharacterAnimInstance::OnClimbing);
    if (MovementComponent->OnUsingDoor.IsAlreadyBound(this, &UPestilenceCharacterAnimInstance::OnUsingDoor))
      MovementComponent->OnUsingDoor.RemoveDynamic(this, &UPestilenceCharacterAnimInstance::OnUsingDoor);

  }
  
  Super::BeginDestroy();
}

void UPestilenceCharacterAnimInstance::AssassinationFinished()
{
  bKillPlayer = false;

  if (CharacterType != EPestilenceTypeCharacter::Guard) return;

  bStartAssassinate = false;
  UBlackboardComponent* Blackboard = Character->GetControllerAI()->GetBlackboard();
  if (!IsValid(Blackboard)) return;

  Blackboard->SetValueAsBool(Keys::StartAssassinate, false);
}

void UPestilenceCharacterAnimInstance::WeaponEquipped()
{
  if (CharacterType != EPestilenceTypeCharacter::Guard) return;

  UBlackboardComponent* Blackboard = Character->GetControllerAI()->GetBlackboard();
  if (!IsValid(Blackboard)) return;

  Blackboard->SetValueAsBool(Keys::Armed, true);
}

void UPestilenceCharacterAnimInstance::OnClimbing(bool _bIsClimbing, bool _bAscending)
{
  bIsClimbing = _bIsClimbing;
  bAscending = _bAscending;
}

void UPestilenceCharacterAnimInstance::OnUsingDoor(bool _bIsUsingDoor, float _InitialYawRotation ,float _CurrentYawRotation)
{
  if (Character->GetPestilencePossessionComponent()->IsPossessed()) return;
  
  if (!_bIsUsingDoor)
  {
    bIsUsingDoor = _bIsUsingDoor;
    MovementComponent->MovementMode = EMovementMode::MOVE_Walking;
    return;
  }
  
  float MinYawRotation = _InitialYawRotation - 45.f;
  float MaxYawRotation = _InitialYawRotation + 45.f;
  
  if (_CurrentYawRotation <= MinYawRotation || _CurrentYawRotation >= MaxYawRotation)
    return;

  bIsUsingDoor = _bIsUsingDoor;
  MovementComponent->MovementMode = EMovementMode::MOVE_None;
}

void UPestilenceCharacterAnimInstance::OnKillPlayer(bool _bCrouched)
{
  bKillPlayer = true;
  bKillCrouched = _bCrouched;
}
