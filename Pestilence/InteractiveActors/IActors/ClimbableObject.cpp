// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbableObject.h"
#include "Kismet/KismetMathLibrary.h"
#include <Pestilence/Components/Movement/PestilenceMovementComponent.h>
#include "../../Components/Interaction/InteractableComponent.h"
#include "../../Components/Interaction/InteractorComponent.h"
#include "Components/BoxComponent.h"
#include "../../Characters/PestilenceCharacter.h" 	
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pestilence/AI/NavArea/NavArea_Interact.h"
#include "Pestilence/Components/AI/PestilenceNavLinkComponent.h"
#include "Components/ArrowComponent.h"
#include "../../Components/Possession/PestilencePossessionComponent.h"

AClimbableObject::AClimbableObject()
{
  Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
  Wall->SetupAttachment(RootComponent);

  InteractableCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Interactable Collider"));
  InteractableCollider->SetupAttachment(Wall);

  EnterPos = CreateDefaultSubobject<USceneComponent>(TEXT("Enter Pos"));
  EnterPos->SetupAttachment(Wall);
  EnterArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Enter Arrow"));
  EnterArrow->SetupAttachment(EnterPos);

  TargetPos = CreateDefaultSubobject<USceneComponent>(TEXT("Target Pos"));
  TargetPos->SetupAttachment(Wall);
  TargetArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Target Arrow"));
  TargetArrow->SetupAttachment(TargetPos);

  InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
  InteractableComponent->InteractableType = EInteractableType::Climb;

  /** Set Link Navigation, to AI use doors
  * Get the defined areas that we will use */
  InteractNavArea = UNavArea_Interact::StaticClass();
  
  PestilenceNavLinkComponent = CreateDefaultSubobject<UPestilenceNavLinkComponent>(TEXT("NavLinkComponent"));
  /** NavLink Points, Start, End & Direction */
  PestilenceNavLinkComponent->SetLinkData(FVector3d(50.f ,370.f, 0.f), FVector3d(50.f ,-100.f, 100.f), ENavLinkDirection::LeftToRight);
  PestilenceNavLinkComponent->SetEnabledArea(UNavArea_Interact::StaticClass()); 
}

void AClimbableObject::BeginPlay()
{
  Super::BeginPlay();

  InteractableComponent->Initialize(InteractableCollider);
  if(!InteractableComponent->OnInteracted.IsAlreadyBound(this, &AClimbableObject::OnInteracted))
    InteractableComponent->OnInteracted.AddDynamic(this, &AClimbableObject::OnInteracted);
}

void AClimbableObject::OnInteracted(bool bResult, UInteractorComponent* Sender)
{
  APestilenceCharacter* PestilenceCharacter = Sender->GetOwner<APestilenceCharacter>();
  if (!PestilenceCharacter) return;
  
  UPestilenceMovementComponent* MovementComponent = PestilenceCharacter->GetPestilenceMovementComponent();
  if (!MovementComponent) return;

  bool bIsFacingClimbable = PestilenceCharacter->GetActorForwardVector().Dot(EnterPos->GetForwardVector()) > 0.f;
  if (PestilenceCharacter->GetActorLocation().Z - MovementComponent->GetHalfHeight() * 2 < GetActorLocation().Z && bIsFacingClimbable)
  {
    if(!MovementComponent->OnTranslating.IsAlreadyBound(this, &AClimbableObject::OnPreClimbEnded))
      MovementComponent->OnTranslating.AddDynamic(this, &AClimbableObject::OnPreClimbEnded);
    MovementComponent->StartTranslation(EnterPos->GetComponentLocation(), EnterPos->GetComponentRotation());
  }
  else if (PestilenceCharacter->GetActorLocation().Z - MovementComponent->GetHalfHeight() * 2 > GetActorLocation().Z &&
    !PestilenceCharacter->GetPestilencePossessionComponent()->IsPossessed() && !bIsFacingClimbable)
  {
    if (!MovementComponent->OnTranslating.IsAlreadyBound(this, &AClimbableObject::OnPreClimbEnded))
      MovementComponent->OnTranslating.AddDynamic(this, &AClimbableObject::OnPreClimbEnded);
    MovementComponent->StartTranslation(TargetPos->GetComponentLocation(), TargetPos->GetComponentRotation());
  }
}

void AClimbableObject::OnPreClimbEnded(bool _bIsTranslating, UPestilenceMovementComponent* _MovementComponent)
{
  if (_bIsTranslating || !_MovementComponent) return;

  if (_MovementComponent->OnTranslating.IsAlreadyBound(this, &AClimbableObject::OnPreClimbEnded))
    _MovementComponent->OnTranslating.RemoveDynamic(this, &AClimbableObject::OnPreClimbEnded);
  APestilenceCharacter* PestilenceCharacter = _MovementComponent->GetOwner<APestilenceCharacter>();
  if (!PestilenceCharacter) return;

  if (PestilenceCharacter->GetActorLocation().Z - _MovementComponent->GetHalfHeight() * 2 < GetActorLocation().Z)
    _MovementComponent->StartClimbing(TargetPos->GetComponentLocation(), bForceCrouch && PestilenceCharacter->GetPestilencePossessionComponent()->IsPossessed(), false);
  else
    _MovementComponent->StartClimbing(EnterPos->GetComponentLocation(), false, true);
  UGameplayStatics::PlaySoundAtLocation(this, ClimbSound, GetActorLocation());
}

