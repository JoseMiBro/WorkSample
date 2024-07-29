// Fill out your copyright notice in the Description page of Project Settings.


#include "IA_HidingSpot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include <Pestilence/Components/Movement/PestilenceMovementComponent.h>
#include "../../Components/Interaction/InteractableComponent.h"
#include "../../Components/Interaction/InteractorComponent.h"
#include "../../Characters/PestilenceCharacter.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"


AIA_HidingSpot::AIA_HidingSpot()
{
  DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
  DefaultSceneRoot->SetupAttachment(RootComponent);

  InteractableCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactable Collider"));
  InteractableCollider->SetupAttachment(DefaultSceneRoot);

  EnterTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Enter Target"));
  EnterTarget->SetupAttachment(DefaultSceneRoot);
  ExitTarget = CreateDefaultSubobject<USceneComponent>(TEXT("Exit Target"));
  ExitTarget->SetupAttachment(DefaultSceneRoot);

  EnterArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Enter Arrow"));
  EnterArrow->SetupAttachment(EnterTarget);
  ExitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Exit Arrow"));
  ExitArrow->SetupAttachment(ExitTarget);

  EnterArrow->ArrowColor = FColor::Blue;
  ExitArrow->ArrowColor = FColor::Red;
  EnterArrow->ArrowSize = 0.4f;
  ExitArrow->ArrowSize = 0.4f;

  InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
  InteractableComponent->InteractableType = EInteractableType::Hiding;

  PassByArea1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Pass By Area 1"));
  PassByArea1->SetupAttachment(DefaultSceneRoot);

  PassByArea2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Pass By Area 2"));
  PassByArea2->SetupAttachment(DefaultSceneRoot);
}

void AIA_HidingSpot::BeginPlay()
{
  Super::BeginPlay();

  InteractableComponent->Initialize(InteractableCollider);
  if(!InteractableComponent->OnInteracted.IsAlreadyBound(this, &AIA_HidingSpot::OnInteracted))
    InteractableComponent->OnInteracted.AddDynamic(this, &AIA_HidingSpot::OnInteracted);
}

void AIA_HidingSpot::OnInteracted(bool bResult, UInteractorComponent* Sender)
{
  APestilenceCharacter* PestilenceCharacter = Sender->GetOwner<APestilenceCharacter>();
  if (!PestilenceCharacter) return;

  UPestilenceMovementComponent* MovementComponent = PestilenceCharacter->FindComponentByClass<UPestilenceMovementComponent>();
  if (!MovementComponent) return;

  FRotator NewRot;
  if (MovementComponent->IsHidden())
    MovementComponent->ExitHideout(ExitTarget->GetComponentLocation(), ExitTarget->GetComponentRotation());
  else
    MovementComponent->PreEnterHideout(ExitTarget->GetComponentLocation(), (-ExitTarget->GetForwardVector()).Rotation(), CapsuleHalfHeight, CapsuleRadius, this);

  UGameplayStatics::PlaySoundAtLocation(this, HideSound, GetActorLocation());
}

void AIA_HidingSpot::OnPreEnterHideout(UPestilenceMovementComponent* _MovementComponent)
{
  _MovementComponent->EnterHideout(EnterTarget->GetComponentLocation(), Type);
}

FVector AIA_HidingSpot::GetEnterPos() const
{
  return EnterTarget->GetComponentLocation();
}

FRotator AIA_HidingSpot::GetEnterDir() const
{
  return EnterTarget->GetComponentRotation();
}

float AIA_HidingSpot::GetCapsuleHalfHeight() const
{
  return CapsuleHalfHeight;
}

float AIA_HidingSpot::GetCapsuleRadius() const
{
  return CapsuleRadius;
}

const UBoxComponent* AIA_HidingSpot::GetPassByArea1()const
{
  return PassByArea1;
}

const UBoxComponent* AIA_HidingSpot::GetPassByArea2() const
{
  return PassByArea2;
}


