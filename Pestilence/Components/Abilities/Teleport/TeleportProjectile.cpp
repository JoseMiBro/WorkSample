// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportProjectile.h"
#include "../../../InteractiveActors/IActors/IA_HidingSpot.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Misc/PestilenceCollisionChannels.h"

void ATeleportProjectile::Initialize(const FVector& _NewPos, const FRotator& _NewRotator, bool _bInHidingSpot)
{
  Super::Initialize(_NewPos, _NewRotator);

  bInHidingSpot = _bInHidingSpot;
}

bool ATeleportProjectile::GetInHidingSpot() const
{
  return bInHidingSpot;
}

AIA_HidingSpot* ATeleportProjectile::GetHidingSpot() const
{
  return bInHidingSpot ? HidingSpot : nullptr;
}

void ATeleportProjectile::BeginPlay()
{
  Super::BeginPlay();

  SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ATeleportProjectile::OnBeginOverlap);
}

void ATeleportProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!OtherActor->IsA(AIA_HidingSpot::StaticClass())) return;

  bInHidingSpot = !bInHidingSpot;
  HidingSpot = bInHidingSpot ? Cast<AIA_HidingSpot>(OtherActor) : nullptr;
}

void ATeleportProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
  UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
  FNavLocation NavLocation;

  UPrimitiveComponent* Component = ImpactResult.GetComponent();
  bool bBounce = MaxFloorAngle != -1 &&
    (FMath::Acos(ImpactResult.Normal.Dot(FVector::UpVector)) > FMath::DegreesToRadians(MaxFloorAngle) ||
      Component->GetCollisionResponseToChannel(ECC_NOTELEPORT) == ECollisionResponse::ECR_Block ||
    (!bInHidingSpot && !NavSystem->ProjectPointToNavigation(ImpactResult.Location, NavLocation)));

  if (bBounce)
    UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
  else
  {
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
    OnProjectileSet.Broadcast(ImpactResult.Location);
    Hide();
  }
}