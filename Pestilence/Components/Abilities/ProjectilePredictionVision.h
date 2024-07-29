// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectilePredictionVision.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PESTILENCE_API UProjectilePredictionVision : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectilePredictionVision();

  void SetProjectileAndMaterial(float _ProjectileRadius, float _MaxFloorAngle, UMaterialInstance* _CylinderMaterial, UMaterialInstance* _SphereMaterial, UMaterialInstance* _DecalMaterial);
  void StartProjectilePrediction(const FVector& _StartPos, const FVector& _StartVel, AActor* _ActorToIgnore);
  void DrawProjectilePrediction(const FVector& _StartPos, const FVector& _StartVel, AActor* _ActorToIgnore, int _CylinderNum, int _NumBounces);
  void FinishedProjectilePrediction(unsigned int _CylinderNum = 0, unsigned int _NumBounces = 0, bool _bGoalVisible = false);
  void ActivatePrediction();
  void DeactivatePrediction();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
  bool bActive = false;

  float ProjectileRadius;
  float MaxFloorAngle;


  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Ability", meta = (AllowPrivateAccess = "true"))
  float PredictionTime = 1.5f;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence | Ability", meta = (AllowPrivateAccess = "true"))
  float PredictionLoopTime = 1.f;

  float LifeTimer = 0.f;

  float TotalTimePredicted = 0.f;
  float PredictionLoopTimer = 0.f;
};
