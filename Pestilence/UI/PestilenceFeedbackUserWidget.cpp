// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceFeedbackUserWidget.h"

#include "Kismet/GameplayStatics.h"

#include "Pestilence/Props/ShadowZone.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/AI/PestilenceAIController.h"
#include "Pestilence/Components/Perception/PestilenceSightComponent.h"
#include "../Components/Possession/PestilencePossessionComponent.h"

bool UPestilenceFeedbackUserWidget::Initialize()
{
  bool ParentSuccess = Super::Initialize();

  if (!ParentSuccess || !GetWorld()) return false;

  BindPlayer();

  return Player != nullptr;
}

void UPestilenceFeedbackUserWidget::OnEndPossess()
{
  Player->GetControllerAI()->FindComponentByClass<UPestilenceSightComponent>()->OnShadowChanged.RemoveDynamic(this, &UPestilenceFeedbackUserWidget::OnPlayerInShadow);
  Player->GetPestilencePossessionComponent()->OnEndPossessEvent().RemoveDynamic(this, &UPestilenceFeedbackUserWidget::OnEndPossess);

  BindPlayer();
}

void UPestilenceFeedbackUserWidget::BindPlayer()
{
  Player = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

  if (!Player || !Player->GetPestilencePossessionComponent()) return;

	APestilenceAIController* AIController = Player->GetControllerAI();
	if (!AIController) return;

	UPestilenceSightComponent* SightComponent = Player->GetControllerAI()->FindComponentByClass<UPestilenceSightComponent>();
  if (!SightComponent) return;

  UPestilencePossessionComponent* PossessionComponent = Player->GetPestilencePossessionComponent();

	SightComponent->OnShadowChanged.AddDynamic(this, &UPestilenceFeedbackUserWidget::OnPlayerInShadow);
  PossessionComponent->OnEndPossessEvent().AddDynamic(this, &UPestilenceFeedbackUserWidget::OnEndPossess);
  OnPlayerInShadow(SightComponent->bOnShadow);
}
