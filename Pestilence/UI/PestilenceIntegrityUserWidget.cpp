// Fill out your copyright notice in the Description page of Project Settings.


#include "PestilenceIntegrityUserWidget.h"

#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

#include "Pestilence/Components/Possession/PestilencePossessionComponent.h"
#include "Pestilence/Characters/PestilenceCharacter.h"

#include "Materials/MaterialParameterCollectionInstance.h"

bool UPestilenceIntegrityUserWidget::Initialize()
{
	bool ParentSuccess = Super::Initialize();

	if (!ParentSuccess || !GetWorld()) return false;

  BindPlayerIntegrity();

  return Player != nullptr;
}

void UPestilenceIntegrityUserWidget::OnEndPossess()
{
	Player->GetPestilencePossessionComponent()->IntegrityChangedDelegate.RemoveDynamic(this, &UPestilenceIntegrityUserWidget::OnIntegrityModified);
	Player->GetPestilencePossessionComponent()->OnEndPossessEvent().RemoveDynamic(this, &UPestilenceIntegrityUserWidget::OnEndPossess);

  BindPlayerIntegrity();
}

void UPestilenceIntegrityUserWidget::SetPostProcess(float amount)
{
	if (IntegrityParameterCollection != nullptr) {
		UMaterialParameterCollectionInstance* IntegrityParameterInstance = GetWorld()->GetParameterCollectionInstance(IntegrityParameterCollection);

		IntegrityParameterInstance->SetScalarParameterValue(OffsetName, amount);
	}
}

void UPestilenceIntegrityUserWidget::BindPlayerIntegrity()
{
  Player = Cast<APestilenceCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

  if (!Player || !Player->GetPestilencePossessionComponent()) return;

  UPestilencePossessionComponent* PossessionComponent = Player->GetPestilencePossessionComponent();

  PossessionComponent->IntegrityChangedDelegate.AddDynamic(this, &UPestilenceIntegrityUserWidget::OnIntegrityModified);
  PossessionComponent->OnEndPossessEvent().AddDynamic(this, &UPestilenceIntegrityUserWidget::OnEndPossess);
}
