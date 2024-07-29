// Fill out your copyright notice in the Description page of Project Settings.


#include "PulseProjectile.h"
#include "Materials/MaterialParameterCollection.h"
#include "Curves/CurveFloat.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "UObject/NameTypes.h"
#include "../../../Misc/PestilenceCollisionChannels.h"
#include "Engine/Engine.h"
#include "Components/MeshComponent.h"
#include "Kismet/GameplayStatics.h"

bool APulseProjectile::GetIsPulseActive()
{
  return bIsPulseActive;
}

void APulseProjectile::BeginPlay()
{
  Super::BeginPlay();

  float PulseCurveMinTime, PulseCurveMaxTime;
  PulseCurve->GetTimeRange(PulseCurveMinTime, PulseCurveMaxTime);
  PulseTime = PulseCurveMaxTime - PulseCurveMinTime;
}

void APulseProjectile::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (!bIsPulseActive) return;

  PulseTimer += DeltaTime;
  PulseTimer = PulseTimer >= PulseTime ? PulseTime : PulseTimer;
  bIsPulseActive = PulseTimer != PulseTime;
  if (!bIsPulseActive)
    OnPulseEnd();

  float CurveFloatValue = PulseCurve->GetFloatValue(PulseTimer);

  UMaterialParameterCollectionInstance* PulseParameterInstance = GetWorld()->GetParameterCollectionInstance(PulseParameterCollection);

  PulseParameterInstance->SetScalarParameterValue(RadiusName, CurveFloatValue);
  PulseParameterInstance->SetScalarParameterValue(BaseReflectName, CurveFloatValue);
}

void APulseProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
  UMaterialParameterCollectionInstance* PulseParameterInstance = GetWorld()->GetParameterCollectionInstance(PulseParameterCollection);
  PulseParameterInstance->SetVectorParameterValue(SpawnPosName, GetActorLocation());

  UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
  UGameplayStatics::PlaySoundAtLocation(this, WaveSound, GetActorLocation());

  DetectActors();
  Hide();
  bIsPulseActive = true;
  PulseTimer = 0.f;

  OnPulseStarted.Broadcast();
}

void APulseProjectile::DetectActors()
{
  TArray<FHitResult> Hits;
  FCollisionShape CollisionShape;
  CollisionShape.SetSphere(PulseRadius);
  GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat(), ECC_PULSEVISION, CollisionShape);
  for(int i = 0; i < Hits.Num(); ++i)
  {
    AActor* HitActor = Hits[i].GetActor();
    TArray<UMeshComponent*> Components; 
    HitActor->GetComponents<UMeshComponent>(Components);
    if(Components.Num() > 0)
    {
      for (int j = 0; j < Components.Num(); ++j)
      {
        Components[j]->SetRenderCustomDepth(true);
        PulseVisibleComponents.Add(Components[j]);
      }
    }
  }
}

void APulseProjectile::OnPulseEnd()
{
  for(int i = 0; i < PulseVisibleComponents.Num(); ++i)
    PulseVisibleComponents[i]->SetRenderCustomDepth(false);

  PulseVisibleComponents.Empty();

  OnPulseEnded.Broadcast();
}
