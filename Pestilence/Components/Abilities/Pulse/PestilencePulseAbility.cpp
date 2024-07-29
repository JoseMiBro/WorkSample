// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilencePulseAbility.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "Camera/CameraComponent.h"
#include "PulseProjectile.h"
#include "../../Possession/PestilencePossessionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../AbilityProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../../Core/PestilenceSaveGameSubsystem.h"
#include "../PestilenceAbilityComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../../Enums/AbilityState.h"
#include "../States/BaseAbilityState.h"
#include "PulseImpact.h"

void UPestilencePulseAbility::Select_Implementation(bool _bPlaySound /*= true*/)
{
  Super::Select_Implementation(_bPlaySound);
}

void UPestilencePulseAbility::Initialize_Implementation(UWorld* _World)
{
  Super::Initialize_Implementation(_World);

  PulseProjectile = _World->SpawnActor<APulseProjectile>(PulseProjectileClass);
  PulseImpact = _World->SpawnActor<APulseImpact>(PulseImpactClass);
  
  PulseProjectile->OnLifeTimeEnded.AddDynamic(this, &UPestilencePulseAbility::OnProjectileLifeTimeEnded);
  PulseProjectile->OnPulseStarted.AddDynamic(this, &UPestilencePulseAbility::OnPulseStarted);
  PulseProjectile->OnPulseEnded.AddDynamic(this, &UPestilencePulseAbility::OnPulseEnded);

  //UGameInstance* GameInstance = _World->GetGameInstance();
  //UPestilenceSaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
  //SaveGameSubsystem->OnLoad.AddDynamic(this, &UPestilencePulseAbility::OnProjectileLifeTimeEnded);

  SetAbilityType(EAbility::Pulse);
}

void UPestilencePulseAbility::StartPrepare_Implementation(bool& _OutResult)
{
  _OutResult = !PulseProjectile->IsActive() && !PulseProjectile->GetIsPulseActive();
  Super::StartPrepare_Implementation(_OutResult);
  if (!_OutResult) return;

  if (bThrowProjectile || GetOwner()->GetPestilencePossessionComponent()->GetCurrentIntegrity() > IntegrityCost)
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(PulseProjectile->GetSimpleCollisionRadius(), PulseProjectile->GetMaxFloorAngle(), CylinderMaterial, SphereMaterial, DecalMaterial);
  else
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(PulseProjectile->GetSimpleCollisionRadius(), PulseProjectile->GetMaxFloorAngle(), NoIntegrityCylinderMaterial, NoIntegritySphereMaterial, NoIntegrityDecalMaterial);
  if (!bThrowProjectile)
    GetOwner()->GetPestilencePossessionComponent()->IntegrityChangedDelegate.AddDynamic(this, &UPestilencePulseAbility::OnIntegrityChanged);

  PulseProjectile->AttachToComponent(GetOwner()->GetAbilityAttachment(), {
    EAttachmentRule::SnapToTarget, true
  });
  GetOwner()->GetProjectilePredictionVision()->ActivatePrediction();
}

void UPestilencePulseAbility::TickPrepare_Implementation(float _DeltaTime, bool& _OutResult)
{
  Super::TickPrepare_Implementation(_DeltaTime, _OutResult);

  _OutResult = !PulseProjectile->IsActive() && !PulseProjectile->GetIsPulseActive();
  if (!_OutResult) return;

  FVector StartPos = GetOwner()->GetAbilityAttachment()->GetComponentLocation();
  FVector StartVel = GetOwner()->GetFirstPersonCameraComponent()->GetForwardVector() * PulseProjectile->GetProjectileComponent()->InitialSpeed;
  GetOwner()->GetProjectilePredictionVision()->StartProjectilePrediction(StartPos, StartVel, GetOwner());
}

void UPestilencePulseAbility::EndPrepare_Implementation()
{
  Super::EndPrepare_Implementation();
  GetOwner()->GetProjectilePredictionVision()->DeactivatePrediction(); 

  if (GetOwner()->GetPestilencePossessionComponent()->IntegrityChangedDelegate.IsAlreadyBound(this, &UPestilencePulseAbility::OnIntegrityChanged))
    GetOwner()->GetPestilencePossessionComponent()->IntegrityChangedDelegate.RemoveDynamic(this, &UPestilencePulseAbility::OnIntegrityChanged);
  
  if (PulseProjectile->IsActive() || PulseProjectile->GetIsPulseActive()) return;
  PulseProjectile->SetVisibility(false);
}

void UPestilencePulseAbility::StartCast_Implementation(bool& _OutResult)
{
  _OutResult = !PulseProjectile->IsActive() && !PulseProjectile->GetIsPulseActive();
  Super::StartCast_Implementation(_OutResult);
  PulseProjectile->SetVisibility(true);
  PulseProjectile->AttachToComponent(GetOwner()->GetAbilityAttachment(), {
    EAttachmentRule::SnapToTarget, true
  });

  bool bOutResult;
  StartPrepare(bOutResult);
}

void UPestilencePulseAbility::TickCast_Implementation(float _DeltaTime, bool& _OutResult)
{
  Super::TickCast_Implementation(_DeltaTime, _OutResult);

  bool bOutResult;
  TickPrepare(_DeltaTime, bOutResult);
}

void UPestilencePulseAbility::EndCast_Implementation(bool& _OutResult)
{
  Super::EndCast_Implementation(_OutResult);

  FVector ProjectilePos = GetOwner()->GetAbilityAttachment()->GetComponentLocation();
  ProjectileRelativePos = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetActorTransform(), ProjectilePos);
  EndPrepare();
}

void UPestilencePulseAbility::CancelCast_Implementation()
{
  Super::CancelCast_Implementation();

  PulseProjectile->SetVisibility(false);
  EndPrepare();
}

void UPestilencePulseAbility::StartAbility_Implementation()
{
  Super::StartAbility_Implementation();
  bThrowProjectile = true;
  PulseProjectile->SetVisibility(true);
  PulseProjectile->AttachToComponent(GetOwner()->GetAbilityAttachment(), {
    EAttachmentRule::SnapToTarget, true
  });

  bool bOutResult;
  StartPrepare(bOutResult);
}

void UPestilencePulseAbility::TickAbility_Implementation(float _DeltaTime)
{
  Super::TickAbility_Implementation(_DeltaTime);

  bool bOutResult;
  if (bThrowProjectile)
    TickPrepare(_DeltaTime, bOutResult);
}

void UPestilencePulseAbility::ProjectileAppear()
{
  Super::ProjectileAppear();
  PulseProjectile->SetVisibility(GetOwner()->GetPestilenceAbilityComponent()->GetCurrentState()->GetStateType() == EAbilityState::Prepare);
}

void UPestilencePulseAbility::UseAbilityNotify()
{
  Super::UseAbilityNotify();
  PulseProjectile->DetachFromActor({ FDetachmentTransformRules::KeepWorldTransform });
  PulseProjectile->SetActorLocation(UKismetMathLibrary::TransformLocation(GetOwner()->GetActorTransform(), ProjectileRelativePos));
  bThrowProjectile = false;
  EndPrepare();
  PulseProjectile->Initialize(GetOwner()->GetAbilityAttachment()->GetComponentLocation(), GetOwner()->GetFirstPersonCameraComponent()->GetComponentRotation());
}

void UPestilencePulseAbility::OnProjectileLifeTimeEnded()
{
  float CurrentIntegrity = GetOwner()->GetPestilencePossessionComponent()->GetCurrentIntegrity();
  CurrentIntegrity += IntegrityCost;
  GetOwner()->GetPestilencePossessionComponent()->SetCurrentIntegrity(CurrentIntegrity);

  if (GetOwner()->GetPestilenceAbilityComponent()->GetIsTryPrepare())
    GetOwner()->GetPestilenceAbilityComponent()->StartPrepare();
}

void UPestilencePulseAbility::OnPulseStarted()
{
  PulseImpact->Initialize(PulseProjectile->GetActorLocation());
}

void UPestilencePulseAbility::OnPulseEnded()
{
  if (GetOwner()->GetPestilenceAbilityComponent()->GetIsTryPrepare())
    GetOwner()->GetPestilenceAbilityComponent()->StartPrepare();
}

void UPestilencePulseAbility::SetNewOwner(APestilenceCharacter* _Owner)
{
  Super::SetNewOwner(_Owner);

  PulseProjectile->SetProjectileOwner(_Owner);
}

void UPestilencePulseAbility::OnIntegrityChanged(float PreviousIntegrity, float CurrentIntegrity)
{
  if(CurrentIntegrity > IntegrityCost)
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(PulseProjectile->GetSimpleCollisionRadius(), PulseProjectile->GetMaxFloorAngle(), CylinderMaterial, SphereMaterial, DecalMaterial);
  else
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(PulseProjectile->GetSimpleCollisionRadius(), PulseProjectile->GetMaxFloorAngle(), NoIntegrityCylinderMaterial, NoIntegritySphereMaterial, NoIntegrityDecalMaterial);
}

