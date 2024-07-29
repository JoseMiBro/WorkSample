// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbilityProjectile.h"
#include "TeleportProjectile.generated.h"

class AIA_HidingSpot;

/**
 * 
 */
UCLASS()
class PESTILENCE_API ATeleportProjectile : public AAbilityProjectile
{
	GENERATED_BODY()

public:
  void Initialize(const FVector& _NewPos, const FRotator& _NewRotator, bool _bInHidingSpot);

  bool GetInHidingSpot() const;
  AIA_HidingSpot* GetHidingSpot() const;
protected:
  virtual void BeginPlay() override;

  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  virtual void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;

  bool bInHidingSpot = false;
  TObjectPtr<AIA_HidingSpot> HidingSpot;

};
