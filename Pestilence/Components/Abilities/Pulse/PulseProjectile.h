// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbilityProjectile.h"
#include "PulseProjectile.generated.h"

class UMaterialParameterCollection;
class UCurveFloat;
class FName;
class UMeshComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPulseStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPulseEnded);

/**
 * 
 */
UCLASS()
class PESTILENCE_API APulseProjectile : public AAbilityProjectile
{
	GENERATED_BODY()

public:
  bool GetIsPulseActive();
  
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnPulseStarted OnPulseStarted;
  UPROPERTY(BlueprintAssignable, Category = "Pestilence|Ability")
	FOnPulseEnded OnPulseEnded;

protected:
  virtual void BeginPlay() override;

private:

  // Called every frame
  virtual void Tick(float DeltaTime) override; 
  
  virtual void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity) override;

  void DetectActors();
  void OnPulseEnd();

  bool bIsPulseActive = false;

  float PulseTime;
  float PulseTimer; 

  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  float PulseRadius;
  
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TObjectPtr<UMaterialParameterCollection> PulseParameterCollection;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TObjectPtr<UCurveFloat> PulseCurve;

  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  FName RadiusName;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  FName BaseReflectName;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  FName SpawnPosName;

  TArray<TObjectPtr<UMeshComponent>> PulseVisibleComponents;

  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TObjectPtr<USoundBase> WaveSound;
};
