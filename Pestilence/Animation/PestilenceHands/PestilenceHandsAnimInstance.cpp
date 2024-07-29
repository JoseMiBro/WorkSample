// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceHandsAnimInstance.h"
#include "../../Characters/PestilenceCharacter.h"
#include "../../Components/Abilities/PestilenceAbilityComponent.h"
#include "../../Components/Movement/PestilenceMovementComponent.h"
#include "../../Components/Abilities/PestilenceAbility.h"
#include "../../Components/TakeDown/TakeDownComponent.h"
#include "../../Components/FailState/FailStateComponent.h"

void UPestilenceHandsAnimInstance::NativeInitializeAnimation()
{
  Character = Cast<APestilenceCharacter>(GetOwningActor());
  if (!Character) return;
  AbilityComponent = Character->GetPestilenceAbilityComponent();
  MovementComponent = Character->GetPestilenceMovementComponent();
}

void UPestilenceHandsAnimInstance::NativeBeginPlay()
{
  if (!IsValid(AbilityComponent) || !IsValid(MovementComponent)) return;

  if(!AbilityComponent->OnAbilityEquipped.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnAbilitySelected))
    AbilityComponent->OnAbilityEquipped.AddDynamic(this, &UPestilenceHandsAnimInstance::OnAbilitySelected);
  if (!AbilityComponent->OnStateChanged.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnStateChanged))
    AbilityComponent->OnStateChanged.AddDynamic(this, &UPestilenceHandsAnimInstance::OnStateChanged);
  if (!MovementComponent->OnCrouch.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnCrouch))
    MovementComponent->OnCrouch.AddDynamic(this, &UPestilenceHandsAnimInstance::OnCrouch);
  if (!MovementComponent->OnTranslating.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnTranslating))
    MovementComponent->OnTranslating.AddDynamic(this, &UPestilenceHandsAnimInstance::OnTranslating);
  if (Character->GetTakeDownComponent() && !Character->GetTakeDownComponent()->OnTakeDownStarted.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnTakeDownStarted))
    Character->GetTakeDownComponent()->OnTakeDownStarted.AddDynamic(this, &UPestilenceHandsAnimInstance::OnTakeDownStarted);
  if (Character->GetFailStateComponent() && !Character->GetFailStateComponent()->OnStartDeathFall.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnStartDeathFall))
    Character->GetFailStateComponent()->OnStartDeathFall.AddDynamic(this, &UPestilenceHandsAnimInstance::OnStartDeathFall);
}

void UPestilenceHandsAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
  if (IsValid(AbilityComponent) && IsValid(AbilityComponent->GetCurrentAbility()))
  {
    bCanReuseAbility = AbilityComponent->GetCurrentAbility()->GetCanReuse();
  }
  if (!IsValid(MovementComponent)) return;

  Velocity = MovementComponent->Velocity;
  GroundSpeed = Velocity.Length();

  bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector(0.f, 0.f, 0.f);

  bIsFalling = MovementComponent->IsFalling();
  bIsRolling = MovementComponent->IsRolling();
}

void UPestilenceHandsAnimInstance::BeginDestroy()
{
  if (Character && IsValid(AbilityComponent) && IsValid(MovementComponent))
  {
    if (AbilityComponent->OnAbilityEquipped.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnAbilitySelected))
      AbilityComponent->OnAbilityEquipped.RemoveDynamic(this, &UPestilenceHandsAnimInstance::OnAbilitySelected);
    if (AbilityComponent->OnStateChanged.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnStateChanged))
      AbilityComponent->OnStateChanged.RemoveDynamic(this, &UPestilenceHandsAnimInstance::OnStateChanged);
    if (MovementComponent->OnCrouch.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnCrouch))
      MovementComponent->OnCrouch.RemoveDynamic(this, &UPestilenceHandsAnimInstance::OnCrouch);
    if (MovementComponent->OnTranslating.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnTranslating))
      MovementComponent->OnTranslating.RemoveDynamic(this, &UPestilenceHandsAnimInstance::OnTranslating);
    if (Character->GetTakeDownComponent() && Character->GetTakeDownComponent()->OnTakeDownStarted.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnTakeDownStarted))
      Character->GetTakeDownComponent()->OnTakeDownStarted.RemoveDynamic(this, &UPestilenceHandsAnimInstance::OnTakeDownStarted);
    if (Character->GetFailStateComponent() && Character->GetFailStateComponent()->OnStartDeathFall.IsAlreadyBound(this, &UPestilenceHandsAnimInstance::OnStartDeathFall))
      Character->GetFailStateComponent()->OnStartDeathFall.RemoveDynamic(this, &UPestilenceHandsAnimInstance::OnStartDeathFall);
  }

  Super::BeginDestroy();
}

void UPestilenceHandsAnimInstance::OnAbilitySelected(EAbility _Ability)
{
  CurrentAbility = _Ability;
  if (!IsValid(AbilityComponent) || !IsValid(AbilityComponent->GetCurrentAbility())) return;
  
  if(AbilityComponent->GetCurrentState() == AbilityComponent->GetPrepareState())
    AbilityComponent->GetCurrentAbility()->ProjectileAppear();
  
}

void UPestilenceHandsAnimInstance::OnStateChanged(EAbilityState _AbilityState)
{
  AbilityState = _AbilityState;
}

void UPestilenceHandsAnimInstance::OnCrouch(bool IsCrouch)
{
  bStartCrouch = IsCrouch;
  bEndCrouch = !IsCrouch;
}

void UPestilenceHandsAnimInstance::CrouchStarted()
{
  bStartCrouch = false;
}

void UPestilenceHandsAnimInstance::CrouchEnded()
{
  bEndCrouch = false;
}

void UPestilenceHandsAnimInstance::OnTranslating(bool _bIsTranslating, UPestilenceMovementComponent* _MovementComponent)
{
  bIsTranslating = _bIsTranslating;
}

void UPestilenceHandsAnimInstance::OnTakeDownStarted()
{
  bTakeDownStarted = true;
}

void UPestilenceHandsAnimInstance::OnStartDeathFall()
{
  bDie = true;
}

void UPestilenceHandsAnimInstance::ProjectileAppear()
{
  if (IsValid(AbilityComponent) && IsValid(AbilityComponent->GetCurrentAbility()))
    AbilityComponent->GetCurrentAbility()->ProjectileAppear();
}
