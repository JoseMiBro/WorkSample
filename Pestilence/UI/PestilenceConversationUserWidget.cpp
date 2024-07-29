// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceConversationUserWidget.h"

#include "Kismet/GameplayStatics.h"

#include "Pestilence/Props/RumourZone.h"

bool UPestilenceConversationUserWidget::Initialize()
{
	bool parentSuccess = Super::Initialize();

	if (!parentSuccess)
	{
		return false;
	}
	/*
	if (GetWorld())
	{
		TArray<AActor*> pRumourZones;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARumourZone::StaticClass(), pRumourZones);

		for (AActor* Actor : pRumourZones)
		{
			ARumourZone* pRumourZone = Cast<ARumourZone>(Actor);

			if (pRumourZone)
			{
				pRumourZone->OnConversationText.AddDynamic(this, &UPestilenceConversationUserWidget::OnTextModified);
				pRumourZone->OnRange.AddDynamic(this, &UPestilenceConversationUserWidget::OnRangeModified);
				pRumourZone->OnDisable.AddDynamic(this, &UPestilenceConversationUserWidget::DisableAll);
				pRumourZone->OnSetOwner.AddDynamic(this, &UPestilenceConversationUserWidget::SetOwner);
			}
		}
	}
	*/
	return true;
}

void UPestilenceConversationUserWidget::InitializeRumourZone(ARumourZone* _RumourZone)
{
  if (!_RumourZone) return;

  _RumourZone->OnConversationText.AddDynamic(this, &UPestilenceConversationUserWidget::OnTextModified);
  _RumourZone->OnRange.AddDynamic(this, &UPestilenceConversationUserWidget::OnRangeModified);
  _RumourZone->OnDisable.AddDynamic(this, &UPestilenceConversationUserWidget::DisableAll);
  _RumourZone->OnSetOwner.AddDynamic(this, &UPestilenceConversationUserWidget::SetOwner);
}

void UPestilenceConversationUserWidget::RemoveRumourZone(ARumourZone* _RumourZone)
{
  if (!_RumourZone) return;

  _RumourZone->OnConversationText.RemoveDynamic(this, &UPestilenceConversationUserWidget::OnTextModified);
  _RumourZone->OnRange.RemoveDynamic(this, &UPestilenceConversationUserWidget::OnRangeModified);
  _RumourZone->OnDisable.RemoveDynamic(this, &UPestilenceConversationUserWidget::DisableAll);
  _RumourZone->OnSetOwner.RemoveDynamic(this, &UPestilenceConversationUserWidget::SetOwner);
}
