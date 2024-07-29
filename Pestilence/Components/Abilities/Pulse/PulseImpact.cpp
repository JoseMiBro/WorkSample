// Fill out your copyright notice in the Description page of Project Settings.


#include "PulseImpact.h"
#include "Components/StaticMeshComponent.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

// Sets default values
APulseImpact::APulseImpact()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
  RootComponent = RootScene;
  
  NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticles"));
  NiagaraParticles->SetupAttachment(RootScene);
}


void APulseImpact::Initialize(const FVector& _NewPos)
{
    SetActorLocation(_NewPos);

  NiagaraParticles->ActivateSystem();
}