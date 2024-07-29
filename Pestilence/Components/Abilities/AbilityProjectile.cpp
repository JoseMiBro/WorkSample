// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../../Misc/PestilenceCollisionChannels.h"
#include "Components/SplineMeshComponent.h" 	
#include "Components/SplineComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "ProjectilePredictionMesh.h"
#include "../../Core/PestilenceGameMode.h"
#include "../../InteractiveActors/IActors/IA_HidingSpot.h"

// Sets default values
AAbilityProjectile::AAbilityProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
  RootComponent = SphereCollision;

  Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
  Ball->SetupAttachment(SphereCollision);

  ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

  ProjectileMovement->OnProjectileBounce.AddDynamic(this, &AAbilityProjectile::OnBounce);
}

void AAbilityProjectile::Initialize(const FVector& _NewPos, const FRotator& _NewRotator)
{
  ProjectileMovement->SetComponentTickEnabled(true);
  SetActorLocation(_NewPos); 
  SetActorRotation(FRotator::ZeroRotator);
  ProjectileMovement->SetVelocityInLocalSpace(_NewRotator.Vector() * ProjectileMovement->InitialSpeed);
  Ball->SetVisibility(true);
  bIsActive = true;
  LifeTimer = 0.f;

  for (int i = 0; i < DynamicMaterials.Num(); ++i)
  {
    DynamicMaterials[i]->SetScalarParameterValue(ScaleInDepthName, ScaleInDepthThrownValue);
  }

  float RandomNum = FMath::RandRange(0.f, 1.f);
  RotationSpeed = FMath::Lerp(MinRotationSpeed, MaxRotationSpeed, RandomNum);
}

bool AAbilityProjectile::IsActive() { return bIsActive; }

const UProjectileMovementComponent* AAbilityProjectile::GetProjectileComponent() const
{
  return ProjectileMovement;
}

float AAbilityProjectile::GetLifeTime() const
{
  return LifeTime;
}

void AAbilityProjectile::SetProjectileOwner(APestilenceCharacter* _Owner)
{
  Owner = _Owner;
}

float AAbilityProjectile::GetMaxFloorAngle() const
{
  return MaxFloorAngle;
}

void AAbilityProjectile::SetVisibility(bool _bVisible)
{
  Ball->SetVisibility(_bVisible);
  if (!_bVisible) return;

  for (int i = 0; i < DynamicMaterials.Num(); ++i)
  {
    DynamicMaterials[i]->SetScalarParameterValue(ScaleInDepthName, ScaleInDepthHandValue);
  }
  Ball->SetRelativeRotation((-Owner->GetActorUpVector()).Rotation());
}

void AAbilityProjectile::EndLife()
{
  OnLifeTimeEnded.Broadcast();
  Hide();
}

void AAbilityProjectile::BeginPlay()
{
  Super::BeginPlay();
  Ball->SetVisibility(false);
  bIsActive = false;
  ProjectileMovement->SetComponentTickEnabled(false);


  const TArray<UMaterialInterface*> Materials = Ball->GetMaterials();
  for(int i = 0; i < Materials.Num(); ++i)
  {
    DynamicMaterials.Add(UMaterialInstanceDynamic::Create(Materials[i], this));
    Ball->SetMaterial(i, DynamicMaterials[i]);
  }
}

void AAbilityProjectile::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (!bIsActive) return;

  FQuat DeltaRotation = FQuat::MakeFromEuler(RotationSpeed * DeltaTime);
  Ball->AddLocalRotation(DeltaRotation);

  LifeTimer += DeltaTime;

  if(LifeTimer >= LifeTime)
  {
    EndLife();
  }
}

void AAbilityProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
  UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
  FNavLocation NavLocation;

  bool bBounce = MaxFloorAngle != -1 &&
    (FMath::Acos(ImpactResult.Normal.Dot(FVector::UpVector)) > FMath::DegreesToRadians(MaxFloorAngle) ||
    !NavSystem->ProjectPointToNavigation(ImpactResult.Location, NavLocation));
  if (bBounce) 
    UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
  else
  {
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
    OnProjectileSet.Broadcast(ImpactResult.Location);
    Hide();
  }
}

void AAbilityProjectile::Hide()
{
  Ball->SetVisibility(false);
  bIsActive = false;
  ProjectileMovement->SetComponentTickEnabled(false);
}
