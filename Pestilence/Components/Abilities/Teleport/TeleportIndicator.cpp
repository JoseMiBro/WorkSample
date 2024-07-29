// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportIndicator.h"
#include "Components/StaticMeshComponent.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ATeleportIndicator::ATeleportIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
  RootComponent = RootScene;
  
  NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticles"));
  NiagaraParticles->SetupAttachment(RootScene);

  NiagaraSplash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSplash"));
  NiagaraSplash->SetupAttachment(RootScene);

  IdleAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("IdleAudioComponent"));
  IdleAudioComponent->SetupAttachment(RootScene);

  FadeAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FadeAudioComponent"));
  FadeAudioComponent->SetupAttachment(RootScene);
}

// Called when the game starts or when spawned
void ATeleportIndicator::BeginPlay()
{
	Super::BeginPlay();

  IdleAudioComponent->SetSound(IdleSound);
  FadeAudioComponent->SetSound(FadeSound);
  Hide();
}

// Called every frame
void ATeleportIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (!bIsActive) return;

  TeleportTimer += DeltaTime;
  NiagaraParticles->SetNiagaraVariableFloat(TEXT("LifeTime"), TeleportTime - TeleportTimer);

  if (TeleportTimer >= TeleportTime)
  {
    OnTimeEnded.Broadcast();
    Hide();
  }

}

void ATeleportIndicator::Initialize(const FVector& _NewPos)
{
  SetActorLocation(_NewPos);

  NiagaraParticles->ActivateSystem();
  NiagaraSplash->ActivateSystem();
  IdleAudioComponent->Play();
  bIsActive = true;
  TeleportTimer = 0.f;
}

void ATeleportIndicator::Hide()
{
  if (TeleportTimer < TeleportTime)
  {
    NiagaraParticles->DeactivateImmediate();
    NiagaraSplash->DeactivateImmediate();
    IdleAudioComponent->FadeOut(AudioFadeOutTime, 0.f);
    FadeAudioComponent->Play();
  }
  bIsActive = false;
}

float ATeleportIndicator::GetTeleportTimer() const
{
  return TeleportTimer;
}

bool ATeleportIndicator::IsActive() const
{
  return bIsActive;
}

