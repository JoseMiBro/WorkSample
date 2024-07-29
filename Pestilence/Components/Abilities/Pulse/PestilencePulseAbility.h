// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PestilenceAbility.h"
#include "PestilencePulseAbility.generated.h"

class APestilenceCharacter;
class APulseProjectile;
class UMaterialParameterCollection;
class APulseImpact;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilencePulseAbility : public UPestilenceAbility
{
  GENERATED_BODY()

public:
  virtual void Select_Implementation(bool _bPlaySound = true) override;
  virtual void Initialize_Implementation(UWorld* _World) override;
  virtual void StartPrepare_Implementation(bool& _OutResult) override;
  virtual void TickPrepare_Implementation(float _DeltaTime, bool& _OutResult) override;
  virtual void EndPrepare_Implementation() override;
  virtual void StartCast_Implementation(bool& _OutResult) override;
  virtual void TickCast_Implementation(float _DeltaTime, bool& _OutResult) override;
  virtual void EndCast_Implementation(bool& _OutResult) override;
  virtual void CancelCast_Implementation() override;
  virtual void StartAbility_Implementation() override;
  virtual void TickAbility_Implementation(float _DeltaTime) override;
  
	virtual void ProjectileAppear() override;
	virtual void UseAbilityNotify() override;
  
  UFUNCTION()
  void OnProjectileLifeTimeEnded();
  UFUNCTION()
  void OnPulseStarted();
  UFUNCTION()
  void OnPulseEnded();

  virtual void SetNewOwner(APestilenceCharacter* _Owner) override;
private:
  UFUNCTION()
  void OnIntegrityChanged(float PreviousIntegrity, float CurrentIntegrity);
  
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<APulseProjectile> PulseProjectileClass;
  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<APulseImpact> PulseImpactClass;
  
  UPROPERTY()
  TObjectPtr<APulseProjectile> PulseProjectile;
  UPROPERTY()
  TObjectPtr<APulseImpact> PulseImpact;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> CylinderMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> SphereMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> DecalMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> NoIntegrityCylinderMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> NoIntegritySphereMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> NoIntegrityDecalMaterial;

  bool bThrowProjectile = false;
  FVector ProjectileRelativePos;
};
