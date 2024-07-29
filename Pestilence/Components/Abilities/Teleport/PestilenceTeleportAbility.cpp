// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceTeleportAbility.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "Camera/CameraComponent.h"
#include "TeleportProjectile.h"
#include "../../Possession/PestilencePossessionComponent.h"
#include "../../Movement/PestilenceMovementComponent.h"
#include "TeleportIndicator.h"
#include "Kismet/GameplayStatics.h"
#include "../AbilityProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../../Core/PestilenceSaveGameSubsystem.h"
#include "Engine/GameInstance.h"
#include "../PestilenceAbilityComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../../InteractiveActors/IActors/IA_HidingSpot.h"
#include "../../../Misc/PestilenceCollisionChannels.h"
#include "../../../Enums/AbilityState.h"
#include "../States/BaseAbilityState.h"

void UPestilenceTeleportAbility::Select_Implementation(bool _bPlaySound /*= true*/)
{
  Super::Select_Implementation(_bPlaySound);

  GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(TeleportProjectile->GetSimpleCollisionRadius(), TeleportProjectile->GetMaxFloorAngle(), CylinderMaterial, SphereMaterial, DecalMaterial);
}

void UPestilenceTeleportAbility::Initialize_Implementation(UWorld* _World)
{
  Super::Initialize_Implementation(_World);

  TeleportProjectile = _World->SpawnActor<ATeleportProjectile>(TeleportProjectileClass);

  TeleportIndicator = _World->SpawnActor<ATeleportIndicator>(TeleportIndicatorClass);

  TeleportProjectile->OnProjectileSet.AddDynamic(this, &UPestilenceTeleportAbility::OnTeleportSet);
  TeleportProjectile->OnLifeTimeEnded.AddDynamic(this, &UPestilenceTeleportAbility::OnProjectileLifeTimeEnded);
  TeleportIndicator->OnTimeEnded.AddDynamic(this, &UPestilenceTeleportAbility::HideTeleport);

  //UGameInstance* GameInstance = _World->GetGameInstance();
  //UPestilenceSaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<UPestilenceSaveGameSubsystem>();
  //SaveGameSubsystem->OnLoad.AddDynamic(this, &UPestilenceTeleportAbility::OnProjectileLifeTimeEnded);
  //SaveGameSubsystem->OnLoad.AddDynamic(this, &UPestilenceTeleportAbility::HideTeleport);

  SetAbilityType(EAbility::Teleport);
}

void UPestilenceTeleportAbility::StartPrepare_Implementation(bool& _OutResult)
{
  _OutResult = !TeleportProjectile->IsActive() && !GetCanReuse();
  Super::StartPrepare_Implementation(_OutResult);
  if (!_OutResult) return;

  if (bThrowProjectile || GetOwner()->GetPestilencePossessionComponent()->GetCurrentIntegrity() > IntegrityCost)
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(TeleportProjectile->GetSimpleCollisionRadius(), TeleportProjectile->GetMaxFloorAngle(), CylinderMaterial, SphereMaterial, DecalMaterial);
  else
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(TeleportProjectile->GetSimpleCollisionRadius(), TeleportProjectile->GetMaxFloorAngle(), NoIntegrityCylinderMaterial, NoIntegritySphereMaterial, NoIntegrityDecalMaterial);
  if (!bThrowProjectile)
    GetOwner()->GetPestilencePossessionComponent()->IntegrityChangedDelegate.AddDynamic(this, &UPestilenceTeleportAbility::OnIntegrityChanged);

  TeleportProjectile->AttachToComponent(GetOwner()->GetAbilityAttachment(), {
    EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true
  });
  GetOwner()->GetProjectilePredictionVision()->ActivatePrediction();
}

void UPestilenceTeleportAbility::TickPrepare_Implementation(float _DeltaTime, bool& _OutResult)
{
  Super::TickPrepare_Implementation(_DeltaTime, _OutResult);

  _OutResult = !TeleportProjectile->IsActive() && !GetCanReuse();
  if (!_OutResult) return;

  FVector StartPos = GetOwner()->GetAbilityAttachment()->GetComponentLocation();
  FVector StartVel = GetOwner()->GetFirstPersonCameraComponent()->GetForwardVector() * TeleportProjectile->GetProjectileComponent()->InitialSpeed;
  GetOwner()->GetProjectilePredictionVision()->StartProjectilePrediction(StartPos, StartVel, GetOwner());
}

void UPestilenceTeleportAbility::EndPrepare_Implementation()
{
  Super::EndPrepare_Implementation();
  GetOwner()->GetProjectilePredictionVision()->DeactivatePrediction(); 
  
  if (GetOwner()->GetPestilencePossessionComponent()->IntegrityChangedDelegate.IsAlreadyBound(this, &UPestilenceTeleportAbility::OnIntegrityChanged))
    GetOwner()->GetPestilencePossessionComponent()->IntegrityChangedDelegate.RemoveDynamic(this, &UPestilenceTeleportAbility::OnIntegrityChanged);

  if (TeleportProjectile->IsActive()) return;
    
  TeleportProjectile->SetVisibility(false);
}

void UPestilenceTeleportAbility::StartCast_Implementation(bool& _OutResult)
{

  _OutResult = !TeleportProjectile->IsActive();
  Super::StartCast_Implementation(_OutResult);
  TeleportProjectile->SetVisibility(true);
  TeleportProjectile->AttachToComponent(GetOwner()->GetAbilityAttachment(), {
    EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true
  });

  bool bOutResult;
  StartPrepare(bOutResult);
}

void UPestilenceTeleportAbility::TickCast_Implementation(float _DeltaTime, bool& _OutResult)
{
  Super::TickCast_Implementation(_DeltaTime, _OutResult);
  bool bOutResult;
  TickPrepare(_DeltaTime, bOutResult);
}

void UPestilenceTeleportAbility::EndCast_Implementation(bool& _OutResult)
{
  Super::EndCast_Implementation(_OutResult);

  FVector ProjectilePos = GetOwner()->GetAbilityAttachment()->GetComponentLocation();
  ProjectileRelativePos = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetActorTransform(), ProjectilePos);
  EndPrepare();
}

void UPestilenceTeleportAbility::CancelCast_Implementation()
{
  Super::CancelCast_Implementation();
  EndPrepare();
}

void UPestilenceTeleportAbility::StartAbility_Implementation()
{
  Super::StartAbility_Implementation();

  bThrowProjectile = !GetCanReuse();
  if (!GetCanReuse())
  {
    bool bOutResult;
    if(bThrowProjectile)
      StartPrepare(bOutResult);

    TeleportProjectile->SetVisibility(true);
    TeleportProjectile->AttachToComponent(GetOwner()->GetAbilityAttachment(), {
      EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true
    });
  }
  else
  {
    bStartingTeleport = true;
    bSkipStartTeleport = (EndTeleportPos - GetOwner()->GetActorLocation()).SizeSquared() <= 4 * MoveAmount * MoveAmount;
    bSkipEndTeleport = bSkipStartTeleport || TeleportProjectile->GetHidingSpot();
    EndTeleportPos.Z += GetOwner()->GetPestilenceMovementComponent()->GetHalfHeight();
    GetOwner()->GetPestilenceMovementComponent()->EnableLook(false);
    GetOwner()->GetPestilenceMovementComponent()->EnableMovement(false);
    GetOwner()->GetPestilenceMovementComponent()->StartLookAt(EndTeleportPos, LookAtTime);
    TeleportDir = (EndTeleportPos - GetOwner()->GetActorLocation()).GetSafeNormal();
  }
}

void UPestilenceTeleportAbility::TickAbility_Implementation(float _DeltaTime)
{
  Super::TickAbility_Implementation(_DeltaTime);

  bool bOutResult;
  if (bThrowProjectile)
    TickPrepare(_DeltaTime, bOutResult);
  else if(bTeleporting)
  {

    FadeTimer += _DeltaTime;
    if (FadeTimer <= FadeInTime)
    {
      if(!bFadeOut)
      {
        GetOwner()->GetPestilencePossessionComponent()->CloseToTargetDelegate.Broadcast();
        bFadeOut = true;
      }
      if(!bSkipStartTeleport)
        GetOwner()->SetActorLocation(StartTeleportPos + TeleportDir * MoveAmount * (FadeTimer / FadeInTime));
    }
    else if (FadeTimer <= FadeInTime + FadeTime)
    {
      if (!bSkipEndTeleport)
        GetOwner()->SetActorLocation(EndTeleportPos - TeleportDir * MoveAmount);
      else
        GetOwner()->SetActorLocation(EndTeleportPos);
    }
    else if (FadeTimer <= FadeInTime + FadeTime + FadeOutTime)
    {
      if (!bFadeIn)
      {
        if (TeleportProjectile->GetHidingSpot())
        {
          AIA_HidingSpot* HidingSpot = TeleportProjectile->GetHidingSpot();
          GetOwner()->GetPestilenceMovementComponent()->EnterHideoutTeleport(HidingSpot->GetEnterPos(), HidingSpot->GetEnterDir(), HidingSpot->GetCapsuleHalfHeight(), HidingSpot->GetCapsuleRadius(), HidingSpot->Type, HidingSpot);
        }
        else if (GetOwner()->GetPestilenceMovementComponent()->IsHidden())
          GetOwner()->GetPestilenceMovementComponent()->ExitHideoutTeleport();

        GetOwner()->GetPestilencePossessionComponent()->TargetReachedDelegate.Broadcast();
        bFadeIn = true;
      }
      if (!bSkipEndTeleport)
        GetOwner()->SetActorLocation(EndTeleportPos - TeleportDir * MoveAmount * (1 - (FadeTimer / (FadeInTime + FadeTime + FadeOutTime))));
    }
    else
      EndTeleporting();
  }
}

void UPestilenceTeleportAbility::EndAbility_Implementation()
{
  Super::EndAbility_Implementation();
}

void UPestilenceTeleportAbility::ProjectileAppear()
{
  Super::ProjectileAppear();
  TeleportProjectile->SetVisibility(!GetCanReuse() && GetOwner()->GetPestilenceAbilityComponent()->GetCurrentState()->GetStateType() == EAbilityState::Prepare);
}

void UPestilenceTeleportAbility::UseAbilityNotify()
{
  Super::UseAbilityNotify();

  if (bThrowProjectile)
    ThrowProjectile();
  else
    StartTeleporting();
}

bool UPestilenceTeleportAbility::GetCanReuse() const
{
  bool bReuse = Super::GetCanReuse();
  return bReuse || bStartingTeleport || bTeleporting;
}

void UPestilenceTeleportAbility::OnTeleportSet(const FVector& _Pos)
{
  EndTeleportPos = _Pos;
  SetCanReuse(true);

  TeleportIndicator->Initialize(EndTeleportPos);
}

void UPestilenceTeleportAbility::OnProjectileLifeTimeEnded()
{
  float CurrentIntegrity = GetOwner()->GetPestilencePossessionComponent()->GetCurrentIntegrity();
  CurrentIntegrity += IntegrityCost;
  GetOwner()->GetPestilencePossessionComponent()->SetCurrentIntegrity(CurrentIntegrity);

  if (GetOwner()->GetPestilenceAbilityComponent()->GetIsTryPrepare())
    GetOwner()->GetPestilenceAbilityComponent()->StartPrepare();
}

void UPestilenceTeleportAbility::SetNewOwner(APestilenceCharacter* _Owner)
{
  Super::SetNewOwner(_Owner);

  TeleportProjectile->SetProjectileOwner(_Owner);
}

void UPestilenceTeleportAbility::OnIntegrityChanged(float PreviousIntegrity, float CurrentIntegrity)
{
  if (CurrentIntegrity > IntegrityCost)
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(TeleportProjectile->GetSimpleCollisionRadius(), TeleportProjectile->GetMaxFloorAngle(), CylinderMaterial, SphereMaterial, DecalMaterial);
  else
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(TeleportProjectile->GetSimpleCollisionRadius(), TeleportProjectile->GetMaxFloorAngle(), NoIntegrityCylinderMaterial, NoIntegritySphereMaterial, NoIntegrityDecalMaterial);
}

void UPestilenceTeleportAbility::ThrowProjectile()
{
  UGameplayStatics::PlaySound2D(this, TeleportThrownSound);
  TeleportProjectile->DetachFromActor({ FDetachmentTransformRules::KeepWorldTransform });
  //TeleportProjectile->Initialize(GetOwner()->GetAbilityAttachment()->GetComponentLocation(), GetOwner()->GetFirstPersonCameraComponent()->GetComponentRotation());

  TeleportProjectile->SetActorLocation(UKismetMathLibrary::TransformLocation(GetOwner()->GetActorTransform(), ProjectileRelativePos));
  FHitResult Hit;

  FVector Start = GetOwner()->GetFirstPersonCameraComponent()->GetComponentLocation();
  FVector End = TeleportProjectile->GetActorLocation();
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(GetOwner());
  CollisionParams.AddIgnoredActor(TeleportProjectile);
  bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PROJECTILE, CollisionParams);
  FVector ThrowLocation = bHit ? Start : End;

  TeleportProjectile->Initialize(ThrowLocation, GetOwner()->GetFirstPersonCameraComponent()->GetComponentRotation(), GetOwner()->GetPestilenceMovementComponent()->IsHidden());

  bThrowProjectile = false;
  EndPrepare();
}

void UPestilenceTeleportAbility::StartTeleporting()
{
  bStartingTeleport = false;
  bTeleporting = true;
  FadeTimer = 0.f;
  StartTeleportPos = GetOwner()->GetActorLocation();

  FHitResult Hit;
  FVector Start = GetOwner()->GetFirstPersonCameraComponent()->GetComponentLocation();
  FVector End = Start + TeleportDir * MoveAmount;
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(GetOwner());
  bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PROJECTILE, CollisionParams);

  bSkipStartTeleport = bSkipStartTeleport || bHit;

  Start = GetOwner()->GetFirstPersonCameraComponent()->GetRelativeLocation() + EndTeleportPos;
  End = Start - TeleportDir * MoveAmount;
  bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PROJECTILE, CollisionParams);

  bSkipEndTeleport = bSkipEndTeleport || bHit;

  bFadeOut = false;
  bFadeIn = false;

  UGameplayStatics::PlaySound2D(this, TeleportSound);
}

void UPestilenceTeleportAbility::EndTeleporting()
{
  bTeleporting = false;
  if(!bSkipEndTeleport)
    GetOwner()->SetActorLocation(EndTeleportPos);
  GetOwner()->GetPestilenceMovementComponent()->EnableLook(true);
  GetOwner()->GetPestilenceMovementComponent()->EnableMovement(true);

  HideTeleport();
}

void UPestilenceTeleportAbility::HideTeleport()
{
  TeleportIndicator->Hide();
  SetCanReuse(bStartingTeleport || bTeleporting);

  if (GetOwner()->GetPestilenceAbilityComponent()->GetIsTryPrepare())
    GetOwner()->GetPestilenceAbilityComponent()->StartPrepare();
}

bool UPestilenceTeleportAbility::IsTeleporting() const
{
  return bTeleporting;
}

const ATeleportIndicator* UPestilenceTeleportAbility::GetTeleportIndicator() const
{
  return TeleportIndicator;
}
