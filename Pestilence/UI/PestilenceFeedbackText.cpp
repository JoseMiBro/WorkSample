// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceFeedbackText.h"
#include "Pestilence/Characters/PestilenceCharacter.h"
#include "Pestilence/Components/InteractiveActor/IActorVisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UPestilenceFeedbackText::NativeConstruct()
{
  Super::NativeConstruct();

  if (GetWorld()) 
  {
    pCharacter = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (pCharacter) 
    {
      pInteractComponent = Cast<UIActorVisionComponent>(pCharacter->GetComponentByClass(UIActorVisionComponent::StaticClass()));
      if (pInteractComponent) 
      {
        pInteractComponent->feedBackTextDelegate.BindDynamic(this, &UPestilenceFeedbackText::SetInteractText);
        pInteractComponent->feedBackTextDelegateExit.BindDynamic(this, &UPestilenceFeedbackText::SetExitInteractText);
      }
    }
  }
}

void UPestilenceFeedbackText::SetInteractText(FString FeedbackText)
{
  InteractText->SetText(FText::FromString(FeedbackText));
  InteractSizeBox->SetVisibility(ESlateVisibility::Visible);
}

void UPestilenceFeedbackText::SetExitInteractText()
{
  InteractSizeBox->SetVisibility(ESlateVisibility::Hidden);
}
