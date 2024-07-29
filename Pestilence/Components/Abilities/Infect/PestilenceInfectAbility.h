// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PestilenceAbility.h"
#include "PestilenceInfectAbility.generated.h"

class APestilenceCharacter;
class AInfectionDragonflies;

/**
 * 
 */
UCLASS()
class PESTILENCE_API UPestilenceInfectAbility : public UPestilenceAbility
{
	GENERATED_BODY()

public:
  virtual void Initialize_Implementation(UWorld* _World) override;
  virtual void StartPrepare_Implementation(bool& _OutResult) override;
  virtual void EndPrepare_Implementation() override;
  virtual void StartCast_Implementation(bool& _OutResult) override;
  virtual void TickCast_Implementation(float _DeltaTime, bool& _OutResult) override;
  virtual void EndCast_Implementation(bool& _OutResult) override;
  virtual void CancelCast_Implementation() override;
  virtual void StartAbility_Implementation() override;
  virtual void EndAbility_Implementation() override;

  virtual void UseAbilityNotify() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  float InfectIntegrity = 25.f;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
  bool bDisableInputOnInfect = true;

  UPROPERTY(VisibleAnywhere, Category = "Pestilence|Ability")
  TObjectPtr<APestilenceCharacter> CurrentTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	TObjectPtr<USoundBase> InfectedSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	float InfectedSoundRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pestilence|Ability")
	float InfectedSoundVolumeMultiplier;

private:
  UFUNCTION()
  void Infect();

	UFUNCTION()
	void GenerateInfectedSound();

  UPROPERTY(EditAnywhere, Category = "Pestilence|Ability")
  TSubclassOf<AInfectionDragonflies> InfectionDragonfliesClass;

  UPROPERTY()
  TObjectPtr<AInfectionDragonflies> InfectionDragonflies;
};
