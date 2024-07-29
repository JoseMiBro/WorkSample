// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceCrouchUserWidget.h"

#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "../Components/Possession/PestilencePossessionComponent.h"

bool UPestilenceCrouchUserWidget::Initialize()
{
  bool ParentSuccess = Super::Initialize();

  if (!ParentSuccess || !GetWorld()) return false;

  BindPlayer();

  return Player != nullptr;
}

void UPestilenceCrouchUserWidget::OnEndPossess()
{
  Player->GetPestilenceMovementComponent()->OnCrouch.RemoveDynamic(this, &UPestilenceCrouchUserWidget::OnCrouchModified);
  Player->GetPestilencePossessionComponent()->OnEndPossessEvent().RemoveDynamic(this, &UPestilenceCrouchUserWidget::OnEndPossess);

  BindPlayer();
  OnCrouchModified(Player->GetPestilenceMovementComponent()->IsCrouching());
}

void UPestilenceCrouchUserWidget::BindPlayer()
{
  Player = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

  if (!Player || !Player->GetPestilenceMovementComponent() || !Player->GetPestilencePossessionComponent()) return;

  UPestilenceMovementComponent* MovementComponent = Player->GetPestilenceMovementComponent();
  UPestilencePossessionComponent* PossessionComponent = Player->GetPestilencePossessionComponent();

  MovementComponent->OnCrouch.AddDynamic(this, &UPestilenceCrouchUserWidget::OnCrouchModified);
  PossessionComponent->OnEndPossessEvent().AddDynamic(this, &UPestilenceCrouchUserWidget::OnEndPossess);
}
