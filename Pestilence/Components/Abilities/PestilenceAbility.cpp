// Fill out your copyright notice in the Description page of Project Settings.

#include "PestilenceAbility.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/Components/Perception/SuspiciousActionSenderComponent.h"

// Sí, es lo que parece.
#include "Pestilence/Components/Abilities/Pickable/PickableAbility.h"
#include "Components/AudioComponent.h"

void UPestilenceAbility::Initialize_Implementation(UWorld* _World)
{
  CastSoundComponent = UGameplayStatics::CreateSound2D(_World, CastSound);
}

void UPestilenceAbility::Select_Implementation(bool _bPlaySound)
{
  if (_bPlaySound)
  {
    UGameplayStatics::PlaySound2D(Owner, SelectSound);
  }
}

void UPestilenceAbility::StartPrepare_Implementation(bool& _OutResult)
{
  if(_OutResult)
    UGameplayStatics::PlaySound2D(Owner, PrepareSound);
}

void UPestilenceAbility::TickPrepare_Implementation(float _DeltaTime, bool& _OutResult)
{

}

void UPestilenceAbility::EndPrepare_Implementation()
{

}

void UPestilenceAbility::StartCast_Implementation(bool& _OutResult)
{
  if (_OutResult && CastSoundComponent)
    CastSoundComponent->Play();

}

void UPestilenceAbility::TickCast_Implementation(float _DeltaTime, bool& _OutResult)
{

}

void UPestilenceAbility::EndCast_Implementation(bool& _OutResult)
{

}

void UPestilenceAbility::CancelCast_Implementation()
{
  if(CastSoundComponent)
  CastSoundComponent->FadeOut(AudioFadeOutTime, 0.f);
}

void UPestilenceAbility::StartAbility_Implementation()
{
  UGameplayStatics::PlaySound2D(Owner, AbilitySound);

  // Perdoname Fer, soy un vago.
  if (AbilityType != EAbility::Pickable)
    Owner->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(GetOwner(), ESuspiciousActionType::DefaultAbility);
}

void UPestilenceAbility::TickAbility_Implementation(float _DeltaTime)
{

}

void UPestilenceAbility::EndAbility_Implementation()
{
  // va
  if (AbilityType != EAbility::Pickable && AbilityType != EAbility::Possession)
    Owner->GetSuspiciousActionSenderComponent()->SubmitSuspiciousAction(GetOwner(), ESuspiciousActionType::DefaultAbility);
}

UWorld* UPestilenceAbility::GetWorld() const
{
  if (Owner)
    return Owner->GetWorld();
  else
    return nullptr;
}

void UPestilenceAbility::SetNewOwner(APestilenceCharacter* _Owner)
{
  Owner = _Owner;
}

APestilenceCharacter* UPestilenceAbility::GetOwner() const { return Owner; }

bool UPestilenceAbility::GetCanReuse() const { return bCanReuse; }
void UPestilenceAbility::SetCanReuse(bool _bCanReuse) { bCanReuse = _bCanReuse; }

EAbility UPestilenceAbility::GetAbilityType() const
{
  return AbilityType;
}

void UPestilenceAbility::ProjectileAppear()
{

}

void UPestilenceAbility::UseAbilityNotify()
{

}

void UPestilenceAbility::SetAbilityType(EAbility _AbilityType)
{
  AbilityType = _AbilityType;
}
