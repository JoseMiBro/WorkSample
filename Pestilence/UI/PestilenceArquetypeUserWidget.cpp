// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceArquetypeUserWidget.h"

#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "../Components/Possession/ArchetypeComponent.h"

bool UPestilenceArquetypeUserWidget::Initialize()
{
  bool ParentSuccess = Super::Initialize();

  if (!ParentSuccess || !GetWorld()) return false;

  BindPlayer();

  return Player != nullptr;
}

void UPestilenceArquetypeUserWidget::OnEndPossess()
{
  Player->GetArchetypeComponent()->OnZoneChanged.RemoveDynamic(this, &UPestilenceArquetypeUserWidget::OnZoneChanged);
  Player->GetPestilencePossessionComponent()->OnEndPossessEvent().RemoveDynamic(this, &UPestilenceArquetypeUserWidget::OnEndPossess);

  BindPlayer();
	OnArchetypeModified(Player->GetArchetypeComponent());
}

void UPestilenceArquetypeUserWidget::BindPlayer()
{
  Player = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

  if (!Player || !Player->GetArchetypeComponent() || !Player->GetPestilencePossessionComponent()) return;

  UArchetypeComponent* ArchetypeComponent = Player->GetArchetypeComponent();
  UPestilencePossessionComponent* PossessionComponent = Player->GetPestilencePossessionComponent();

	ArchetypeComponent->OnZoneChanged.AddDynamic(this, &UPestilenceArquetypeUserWidget::OnZoneChanged);
  PossessionComponent->OnEndPossessEvent().AddDynamic(this, &UPestilenceArquetypeUserWidget::OnEndPossess);
}
