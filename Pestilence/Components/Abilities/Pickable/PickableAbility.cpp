// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableAbility.h"
#include "../../../Characters/PestilenceCharacter.h"
#include "../../Interaction/InventoryComponent.h"
#include "Camera/CameraComponent.h"

void UPickableAbility::Select_Implementation(bool _bPlaySound /*= true*/)
{
  Super::Select_Implementation(_bPlaySound);

  if (auto* PickableMeshBase = GetOwner()->GetInventoryComponent()->GetCurrentPickable()->GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())) 
  {
    UStaticMeshComponent* PickableMesh = Cast<UStaticMeshComponent>(PickableMeshBase);
    float ProjectileRadius = 0.f;
    FKAggregateGeom AggGeom = PickableMesh->GetStaticMesh()->GetBodySetup()->AggGeom;
    if (AggGeom.SphereElems.Num() != 0)
      ProjectileRadius = AggGeom.SphereElems[0].Radius;
    else if (AggGeom.BoxElems.Num() != 0)
    {
      ProjectileRadius = FMath::Max3(AggGeom.BoxElems[0].X, AggGeom.BoxElems[0].Y, AggGeom.BoxElems[0].Z);
      ProjectileRadius /= 2;
    }
    GetOwner()->GetProjectilePredictionVision()->SetProjectileAndMaterial(ProjectileRadius, -1.f, CylinderMaterial, SphereMaterial, DecalMaterial);
  }

}

void UPickableAbility::Initialize_Implementation(UWorld* _World)
{
  Super::Initialize_Implementation(_World);

  SetAbilityType(EAbility::Pickable);
}

void UPickableAbility::StartPrepare_Implementation(bool& _OutResult)
{
  Super::StartPrepare_Implementation(_OutResult);

  GetOwner()->GetProjectilePredictionVision()->ActivatePrediction();
}

void UPickableAbility::TickPrepare_Implementation(float _DeltaTime, bool& _OutResult)
{
  Super::TickPrepare_Implementation(_DeltaTime, _OutResult);
  UInventoryComponent* InventoryComponent = GetOwner()->GetInventoryComponent();
  FVector StartPos = GetOwner()->GetInventoryAttachment()->GetComponentLocation();
  FVector StartVel = InventoryComponent->LaunchSpeed * GetOwner()->GetFirstPersonCameraComponent()->GetForwardVector();

  GetOwner()->GetProjectilePredictionVision()->StartProjectilePrediction(StartPos, StartVel, GetOwner());
}

void UPickableAbility::EndPrepare_Implementation()
{
  Super::EndPrepare_Implementation();

  GetOwner()->GetProjectilePredictionVision()->DeactivatePrediction();
}

void UPickableAbility::EndAbility_Implementation()
{
  Super::EndAbility_Implementation();

  GetOwner()->GetInventoryComponent()->DropPickable(true);
}