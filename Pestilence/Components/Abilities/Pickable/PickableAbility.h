// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PestilenceAbility.h"
#include "PickableAbility.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPickableAbility : public UPestilenceAbility
{
	GENERATED_BODY()

  virtual void Select_Implementation(bool _bPlaySound = true) override;
  virtual void Initialize_Implementation(UWorld* _World) override;
  virtual void StartPrepare_Implementation(bool& _OutResult) override;
  virtual void TickPrepare_Implementation(float _DeltaTime, bool& _OutResult) override;
  virtual void EndPrepare_Implementation() override;
  virtual void EndAbility_Implementation() override;

private:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> CylinderMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> SphereMaterial;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pestilence|Ability", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInstance> DecalMaterial;
};
