// Fill out your copyright notice in the Description page of Project Settings.


#include "InfectionDragonflies.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

// Sets default values
AInfectionDragonflies::AInfectionDragonflies()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
  RootComponent = RootScene;

  NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticles"));
  NiagaraParticles->SetupAttachment(RootScene);
}

// Called when the game starts or when spawned
void AInfectionDragonflies::BeginPlay()
{
	Super::BeginPlay();
  Hide();
}

// Called every frame
void AInfectionDragonflies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (!bIsFlying) return;

  SetActorRotation((Target->GetActorLocation() - GetActorLocation()).Rotation());
  SetActorLocation(GetActorLocation() + DeltaTime * Speed * GetActorForwardVector());

  if ((Target->GetActorLocation() - GetActorLocation()).SizeSquared() < MinDistance * MinDistance)
  {
    OnTargetReached.Broadcast();
    Hide();
  }
}

void AInfectionDragonflies::StartFlying(const FVector& _StartPos, const AActor* _Target)
{
  Target = _Target;
	bIsFlying = true;
  SetActorLocation(_StartPos);
  SetActorRotation((_Target->GetActorLocation() - _StartPos).Rotation());
  NiagaraParticles->ActivateSystem();
}

void AInfectionDragonflies::Hide()
{
  NiagaraParticles->DeactivateImmediate();
  bIsFlying = false;
}

bool AInfectionDragonflies::IsFlying() const { return bIsFlying; }
