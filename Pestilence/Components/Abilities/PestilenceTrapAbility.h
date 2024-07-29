// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PestilenceAbility.h"
#include "PestilenceTrapAbility.generated.h"

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceTrapAbility : public UPestilenceAbility
{
	GENERATED_BODY()
	
  virtual void Initialize_Implementation(UWorld* _World) override;
  virtual void StartCast_Implementation(bool& _OutResult) override;
  virtual void TickCast_Implementation(float _DeltaTime, bool& _OutResult) override;
  virtual void EndCast_Implementation(bool& _OutResult) override;
  virtual void CancelCast_Implementation() override;
  virtual void StartAbility_Implementation() override;
  virtual void TickAbility_Implementation(float _DeltaTime) override;
  virtual void EndAbility_Implementation() override;
};
