// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PulseImpact.generated.h"

class USceneComponent;
class UNiagaraComponent;

UCLASS()
class PESTILENCE_API APulseImpact : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APulseImpact();

  void Initialize(const FVector& _NewPos);
private:

  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<USceneComponent> RootScene;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> NiagaraParticles;
};
