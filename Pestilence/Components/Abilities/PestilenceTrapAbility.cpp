// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceTrapAbility.h"

void UPestilenceTrapAbility::Initialize_Implementation(UWorld* _World)
{
  Super::Initialize_Implementation(_World);
}

void UPestilenceTrapAbility::StartCast_Implementation(bool& _OutResult)
{
  Super::StartCast_Implementation(_OutResult);
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("StartCastTrap"));

  _OutResult = _OutResult && true;
}

void UPestilenceTrapAbility::TickCast_Implementation(float _DeltaTime, bool& _OutResult)
{
  Super::TickCast_Implementation(_DeltaTime, _OutResult);
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("CastingTrap"));

  _OutResult = _OutResult && true;
}

void UPestilenceTrapAbility::EndCast_Implementation(bool& _OutResult)
{
  Super::EndCast_Implementation(_OutResult);
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("EndCastTrap"));
}

void UPestilenceTrapAbility::CancelCast_Implementation()
{
  Super::CancelCast_Implementation();
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("CancelCastTrap"));
}

void UPestilenceTrapAbility::StartAbility_Implementation()
{
  Super::StartAbility_Implementation();
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("StartAbilityTrap"));
}

void UPestilenceTrapAbility::TickAbility_Implementation(float _DeltaTime)
{
  Super::TickAbility_Implementation(_DeltaTime);
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("TickAbilityTrap"));
}

void UPestilenceTrapAbility::EndAbility_Implementation()
{
  Super::EndAbility_Implementation();
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("EndAbilityTrap"));
}
