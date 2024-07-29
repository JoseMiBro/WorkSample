// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/UI/PestilenceDFUserWidget.h"

#include "Kismet/GameplayStatics.h"

#include "Pestilence/UI/PestilenceHubManager.h"
#include "Pestilence/UI/PestilenceDiaryUserWidget.h"

bool UPestilenceDFUserWidget::Initialize()
{
	bool parentSuccess = Super::Initialize();
	if (!parentSuccess)
	{
		return parentSuccess;
	}

	if (GetWorld())
	{
		TArray<AActor*> HubsManager;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APestilenceHubManager::StaticClass(), HubsManager);

		for (AActor* Actor : HubsManager)
		{
			APestilenceHubManager* PestilenceHubManager = Cast<APestilenceHubManager>(Actor);
			if (PestilenceHubManager) 
			{
				UPestilenceDiaryUserWidget* PestilenceDiaryUserWidget = PestilenceHubManager->DiaryMenuWidgetInstance;
				if (PestilenceDiaryUserWidget) 
				{
					PestilenceDiaryUserWidget->OnImportantTextFeedback.AddDynamic(this, &UPestilenceDFUserWidget::OnImportantModified);
				}
			}
		}
	}

	return true;
}
