// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePredictionVision.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/GameplayStatics.h"
#include "../../Core/PestilenceGameMode.h"
#include "ProjectilePredictionMesh.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "../../Misc/PestilenceCollisionChannels.h"
#include "Components/DecalComponent.h"
#include "../../InteractiveActors/IActors/IA_HidingSpot.h"

// Sets default values for this component's properties
UProjectilePredictionVision::UProjectilePredictionVision()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UProjectilePredictionVision::SetProjectileAndMaterial(float _ProjectileRadius, float _MaxFloorAngle, UMaterialInstance* _CylinderMaterial, UMaterialInstance* _SphereMaterial, UMaterialInstance* _DecalMaterial)
{
  ProjectileRadius = _ProjectileRadius;
  MaxFloorAngle = _MaxFloorAngle;

  APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
  TArray<AProjectilePredictionMesh*> ProjectilePredictionMeshes = GameMode->GetProjectilePredictionMeshes();
  for (int i = 0; i < ProjectilePredictionMeshes.Num(); ++i)
  {
    ProjectilePredictionMeshes[i]->DefaultMesh->SetMaterial(0, _CylinderMaterial);
  }
  TArray<AProjectilePredictionMesh*> ProjectilePredictionBounceMeshes = GameMode->GetProjectilePredictionBounceMeshes();
  for (int i = 0; i < ProjectilePredictionBounceMeshes.Num(); ++i)
  {
    ProjectilePredictionBounceMeshes[i]->DefaultMesh->SetMaterial(0, _SphereMaterial);
  }
  GameMode->GetProjectilePredictionGoalMesh()->DefaultMesh->SetMaterial(0, _SphereMaterial);
  UDecalComponent* DecalComponent = Cast<UDecalComponent>(GameMode->GetProjectilePredictionGoalMesh()->GetComponentByClass(UDecalComponent::StaticClass()));
  if (DecalComponent)
    DecalComponent->SetMaterial(0, _DecalMaterial);
}
  
void UProjectilePredictionVision::StartProjectilePrediction(const FVector& _StartPos, const FVector& _StartVel, AActor* _ActorToIgnore)
{
  TotalTimePredicted = 0.f;
  DrawProjectilePrediction(_StartPos, _StartVel, _ActorToIgnore, 0, 0);
}

void UProjectilePredictionVision::DrawProjectilePrediction(const FVector& _StartPos, const FVector& _StartVel, AActor* _ActorToIgnore, int _CylinderNum, int _NumBounces)
{
  FPredictProjectilePathParams PredictParams = FPredictProjectilePathParams(ProjectileRadius, _StartPos, _StartVel, PredictionTime - TotalTimePredicted, ECC_PROJECTILE, _ActorToIgnore);

  FPredictProjectilePathResult PredictResult;
  UGameplayStatics::PredictProjectilePath(GetOwner(), PredictParams, PredictResult);

  APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
  TArray<AProjectilePredictionMesh*> ProjectilePredictionMeshes = GameMode->GetProjectilePredictionMeshes(); 
  int NewCylinderNum = _CylinderNum + PredictResult.PathData.Num();
  int i = _CylinderNum;
  float PredictionLoopRatio = PredictionLoopTimer / PredictionLoopTime;
  while (i < NewCylinderNum && i < ProjectilePredictionMeshes.Num())
  {
    ProjectilePredictionMeshes[i]->SetVisible(true);

    FPredictProjectilePathPointData NextPathPointData;
    if (i + 1 < ProjectilePredictionMeshes.Num() && i - _CylinderNum + 1 < PredictResult.PathData.Num())
      NextPathPointData = PredictResult.PathData[i - _CylinderNum + 1];
    else
      NextPathPointData = PredictResult.LastTraceDestination;
    FVector Location = FMath::Lerp<FVector>(PredictResult.PathData[i - _CylinderNum].Location, NextPathPointData.Location, PredictionLoopRatio);
    FVector Velocity = FMath::Lerp<FVector>(PredictResult.PathData[i - _CylinderNum].Velocity, NextPathPointData.Velocity, PredictionLoopRatio);
    ProjectilePredictionMeshes[i]->SetActorLocation(Location);
    ProjectilePredictionMeshes[i]->SetActorRotation(Velocity.Rotation());

    ++i;
  }
  if (PredictResult.HitResult.bBlockingHit)
  {
    FPredictProjectilePathPointData LastPoint = PredictResult.PathData.Last(0);

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation NavLocation;

    FHitResult Hit;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(_ActorToIgnore);
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, _StartPos, LastPoint.Location, ECC_INTERACTABLE, CollisionParams);
    bHit = bHit && Hit.GetActor()->IsA(AIA_HidingSpot::StaticClass());

    UPrimitiveComponent* Component = PredictResult.HitResult.GetComponent();
    bool bBounce = MaxFloorAngle > 0.f &&
      (FMath::Acos(PredictResult.HitResult.Normal.Dot(FVector::UpVector)) > FMath::DegreesToRadians(MaxFloorAngle) ||
        Component->GetCollisionResponseToChannel(ECC_NOTELEPORT) == ECollisionResponse::ECR_Block ||
        (!bHit && !NavSystem->ProjectPointToNavigation(PredictResult.HitResult.Location, NavLocation)));
    if (bBounce)
    {
      TotalTimePredicted += LastPoint.Time;
      if (0.05f < LastPoint.Time && TotalTimePredicted < PredictionTime)
      {
        // Siendo r reflejo, v velocidad y n normal.
        // r = v - 2(v * n)n
        FVector NewVelocity = LastPoint.Velocity - 2.f * FVector::DotProduct(LastPoint.Velocity, PredictResult.HitResult.Normal) * PredictResult.HitResult.Normal;

        if(_NumBounces < GameMode->GetProjectilePredictionBounceMeshes().Num())
        {
          GameMode->GetProjectilePredictionBounceMeshes()[_NumBounces]->SetVisible(true);
          GameMode->GetProjectilePredictionBounceMeshes()[_NumBounces]->SetActorLocation(LastPoint.Location);
          ++_NumBounces;
        }

        DrawProjectilePrediction(LastPoint.Location, NewVelocity, PredictResult.HitResult.GetActor(), NewCylinderNum, _NumBounces);
      }
      else
        FinishedProjectilePrediction(NewCylinderNum, _NumBounces, false);
    }
    else
    {
      FVector Normal = PredictResult.HitResult.Normal;
      GameMode->GetProjectilePredictionGoalMesh()->SetVisible(true);
      GameMode->GetProjectilePredictionGoalMesh()->SetActorLocation(PredictResult.HitResult.ImpactPoint);

      FinishedProjectilePrediction(NewCylinderNum, _NumBounces, true);
    }
  }
  else
    FinishedProjectilePrediction(NewCylinderNum, _NumBounces, false);
}

void UProjectilePredictionVision::FinishedProjectilePrediction(unsigned int _CylinderNum /*= 0*/, unsigned int _NumBounces /*= 0*/, bool _bGoalVisible /*= false*/)
{
  APestilenceGameMode* GameMode = GetWorld()->GetAuthGameMode<APestilenceGameMode>();
  TArray<AProjectilePredictionMesh*> ProjectilePredictionMeshes = GameMode->GetProjectilePredictionMeshes();
  for (int i = _CylinderNum; i < ProjectilePredictionMeshes.Num(); ++i)
  {
    ProjectilePredictionMeshes[i]->SetVisible(false);
  }
  TArray<AProjectilePredictionMesh*> ProjectilePredictionBounceMeshes = GameMode->GetProjectilePredictionBounceMeshes();
  for (int i = _NumBounces; i < ProjectilePredictionBounceMeshes.Num(); ++i)
  {
    ProjectilePredictionBounceMeshes[i]->SetVisible(false);
  }
  GameMode->GetProjectilePredictionGoalMesh()->SetVisible(_bGoalVisible);
}

void UProjectilePredictionVision::ActivatePrediction()
{
  bActive = true;
}

void UProjectilePredictionVision::DeactivatePrediction()
{
  bActive = false;
  FinishedProjectilePrediction(0, 0, false);
}

// Called when the game starts
void UProjectilePredictionVision::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UProjectilePredictionVision::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if(bActive)
  {
    PredictionLoopTimer += DeltaTime;
    PredictionLoopTimer = PredictionLoopTimer >= PredictionLoopTime ? PredictionLoopTimer - PredictionLoopTime : PredictionLoopTimer;
  }
}

