// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceSprintUserWidget.h"

#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

#include "Pestilence/Components/Movement/PestilenceMovementComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "../Components/Possession/PestilencePossessionComponent.h"

bool UPestilenceSprintUserWidget::Initialize()
{
  bool ParentSuccess = Super::Initialize();

  if (!ParentSuccess || !GetWorld()) return false;

  BindPlayer();

  return Player != nullptr;
}

void UPestilenceSprintUserWidget::OnEndPossess()
{
  //Player->GetPestilenceMovementComponent()->OnSprint.RemoveDynamic(this, &UPestilenceSprintUserWidget::OnSprintModified);
  //Player->GetPestilencePossessionComponent()->OnEndPossessEvent().RemoveDynamic(this, &UPestilenceSprintUserWidget::OnEndPossess);

  //BindPlayer();
  //OnSprintModified(Player->GetPestilenceMovementComponent()->IsWalking());
}

void UPestilenceSprintUserWidget::BindPlayer()
{
  //Player = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

  //if (!Player || !Player->GetPestilenceMovementComponent() || !Player->GetPestilencePossessionComponent()) return;

	//UPestilenceMovementComponent* MovementComponent = Player->GetPestilenceMovementComponent();
  //UPestilencePossessionComponent* PossessionComponent = Player->GetPestilencePossessionComponent();

  //MovementComponent->OnSprint.AddDynamic(this, &UPestilenceSprintUserWidget::OnSprintModified);
  //PossessionComponent->OnEndPossessEvent().AddDynamic(this, &UPestilenceSprintUserWidget::OnEndPossess);
}
